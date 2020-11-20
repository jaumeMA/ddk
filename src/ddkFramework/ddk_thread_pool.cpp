#include "ddk_thread_pool.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_impl.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

worker_thread_impl::worker_thread_impl()
: m_funcToExecute(none)
, m_state(Idle)
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condVar, NULL);

	pthread_attr_t	attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&m_thread, &attr, threadFunc, this);

	pthread_attr_destroy(&attr);
}
worker_thread_impl::~worker_thread_impl()
{
	m_state = Stopped;

	void *res = NULL;
	pthread_join(m_thread,&res);

	pthread_cond_destroy(&m_condVar);
	pthread_mutex_destroy(&m_mutex);
}
void worker_thread_impl::start(const ddk::function<void()>& i_callable, yielder* i_yielder)
{
	if (m_state != Running)
	{
		pthread_mutex_lock(&m_mutex);

		m_funcToExecute = i_callable;

		if(i_yielder)
		{
			m_yielder = i_yielder;
		}

		pthread_cond_signal(&m_condVar);

		pthread_mutex_unlock(&m_mutex);
	}
}
void worker_thread_impl::stop()
{
	if(m_state == Running)
	{
		//we are using lock/unlock operations just t owait until current function execution ends, s_yielder is thread local theres no need to protect it with mutexes
		//we set it there just to avoid compiler applies some kind of optimization
		pthread_mutex_lock(&m_mutex);

		pthread_mutex_unlock(&m_mutex);
	}
}
bool worker_thread_impl::joinable() const
{
	return m_state == Running;
}
void worker_thread_impl::setExiting(bool i_exiting)
{
	//TBD
}
void worker_thread_impl::execute()
{
	while(m_state != Stopped)
	{
		pthread_mutex_lock(&m_mutex);

		if (m_funcToExecute)
		{
			const ddk::function<void()> funcToExecute = m_funcToExecute.extract();

			m_state = Running;

			thread_impl_interface::set_yielder(m_yielder);

			funcToExecute();

			thread_impl_interface::clear_yielder();

			m_state = Idle;
		}
		else
		{
			pthread_cond_wait(&m_condVar,&m_mutex);
		}

		pthread_mutex_unlock(&m_mutex);
	}
}
bool worker_thread_impl::set_affinity(const cpu_set_t& i_set)
{
	return pthread_setaffinity_np(m_thread,sizeof(cpu_set_t),&i_set) == 0;
}
thread_id_t worker_thread_impl::get_id() const
{
  return get_thread_id(m_thread);
}

}

thread_pool::thread_pool(Policy i_policy, size_t i_initialSize)
: m_policy(i_policy)
{
	m_availableThreads.reserve(i_initialSize);

	for(size_t threadIndex=0;threadIndex<i_initialSize;++threadIndex)
	{
		m_availableThreads.push_back(new detail::worker_thread_impl());
	}
}
thread_pool::thread_pool(thread_pool&& other)
: m_availableThreads(std::move(other.m_availableThreads))
, m_underUseThreads(std::move(other.m_underUseThreads))
, m_policy(other.m_policy)
{
}
thread_pool::~thread_pool()
{
	DDK_ASSERT(m_underUseThreads.empty(), "Trying to remove pool while threads under use");

	thread_container::iterator itThread = m_availableThreads.begin();
	for(;itThread!=m_underUseThreads.end();++itThread)
	{
		delete(*itThread);
	}
}
thread_pool::acquire_result<thread> thread_pool::aquire_thread()
{
	if(m_availableThreads.empty())
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			m_availableThreads.push_back(new detail::worker_thread_impl());
		}
		else
		{
			return make_error<acquire_result<thread>>(NoThreadAvailable);
		}
	}

	thread_container::iterator itThread = m_availableThreads.begin();

	detail::thread_impl_interface* acquiredThread = *itThread;

	m_availableThreads.erase(itThread);

	m_underUseThreads.push_back(acquiredThread);

	return make_result<acquire_result<thread>>(as_unique_reference(acquiredThread,static_cast<const IReferenceWrapperDeleter&>(*this)));
}
thread_pool::acquire_result<thread_sheaf> thread_pool::acquire_sheaf(size_t i_size)
{
	if(m_availableThreads.size() < i_size)
	{
		//depending on the policy
		if(m_policy == GrowsOnDemand)
		{
			m_availableThreads.reserve(i_size);

			const size_t missingFibers = i_size - m_availableThreads.size();
			for(size_t fiberIndex=0;fiberIndex<missingFibers;++fiberIndex)
			{
				m_availableThreads.emplace_back(new detail::worker_thread_impl());
			}
		}
		else
		{
			return make_error<acquire_result<thread_sheaf>>(NoThreadAvailable);
		}
	}

	thread_sheaf threadSheaf;
	threadSheaf.m_threadCtr.reserve(i_size);
	m_underUseThreads.reserve(m_underUseThreads.size() + i_size);

	thread_container::iterator itThread = m_availableThreads.begin();
	for(size_t threadIndex=0;threadIndex<i_size;++threadIndex,++itThread)
	{
		detail::thread_impl_interface* acquiredThread = *itThread;

		threadSheaf.m_threadCtr.push_back(as_unique_reference(acquiredThread,static_cast<const IReferenceWrapperDeleter&>(*this)));

		m_underUseThreads.push_back(acquiredThread);
	}

	m_availableThreads.erase(m_availableThreads.begin(),m_availableThreads.begin()+i_size);

	return make_result<acquire_result<thread_sheaf>>(std::move(threadSheaf));
}
void thread_pool::Deallocate(const void* i_object) const
{
	thread_container::iterator itThread = m_underUseThreads.begin();
	for(;itThread!=m_underUseThreads.end();++itThread)
	{
		if(*itThread == i_object)
		{
			m_underUseThreads.erase(itThread);

			m_availableThreads.push_back(*itThread);

			return;
		}
	}

	DDK_FAIL("Trying to remove non existent thread from pool");
}

}
