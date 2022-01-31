#include "ddk_thread_pool.h"
#include "ddk_thread_pool.h"
#include "ddk_reference_wrapper.h"
#include "ddk_reference_wrapper.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

worker_thread_impl::worker_thread_impl()
: m_state(Idle)
, m_mutex(MutexType::Recursive)
{
	pthread_attr_t	attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&m_thread, &attr, threadFunc, this);

	pthread_attr_destroy(&attr);
}
worker_thread_impl::~worker_thread_impl()
{
	if(m_mutex.try_lock())
	{
		m_state = Stopped;

		m_condVar.notify_all();

		m_mutex.unlock();
	}
	else
	{
		m_state = Stopped;

		m_condVar.notify_all();
	}

	void *res = NULL;
	pthread_join(m_thread,&res);
}
worker_thread_impl::start_result worker_thread_impl::start(const ddk::function<void()>& i_callable, yielder* i_yielder)
{
	if (m_state != Running)
	{
		mutex_guard lg(m_mutex);

		m_funcToExecute = i_callable;

		if(i_yielder)
		{
			m_yielder = i_yielder;
		}

		m_condVar.notify_one();

		return success;
	}
	else
	{
		return ddk::make_error<start_result>(StartErrorCode::StartNotAvailable,"Thread is alreday started.");
	}
}
worker_thread_impl::stop_result worker_thread_impl::stop()
{
	//worker threads are effectively stopped during detsruction
	mutex_guard lg(m_mutex);

	if(m_state == Running)
	{
		m_condVar.wait(m_mutex);
	}

	return success;
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
	mutex_guard lg(m_mutex);

	while(m_state != Stopped)
	{
		if (m_funcToExecute != nullptr)
		{
			m_state = Running;

			thread_impl_interface::set_yielder(m_yielder);

			eval(m_funcToExecute);

			m_state = Idle;

			m_funcToExecute = nullptr;

			thread_impl_interface::clear_yielder();

			m_condVar.notify_one();
		}

		if(m_state != Stopped)
		{
			m_condVar.wait(m_mutex);
		}
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

thread_pool::thread_pool(Policy i_policy, size_t i_maxNumThreads)
: m_policy(i_policy)
, m_maxNumThreads(i_maxNumThreads)
{
	size_t initialSize = (m_policy == FixedSize) ? i_maxNumThreads : 1;

	m_availableThreads.reserve(initialSize);

	for(size_t threadIndex=0;threadIndex< initialSize;++threadIndex)
	{
		m_availableThreads.push_back(new detail::worker_thread_impl());
	}
}
thread_pool::thread_pool(thread_pool&& other)
: m_policy(other.m_policy)
, m_maxNumThreads(other.m_maxNumThreads)
, m_availableThreads(std::move(other.m_availableThreads))
, m_underUseThreads(std::move(other.m_underUseThreads))
{
}
thread_pool::~thread_pool()
{
	m_mutex.lock();
		
	thread_in_use_container::iterator itInUseThread = m_underUseThreads.begin();
	for(; itInUseThread != m_underUseThreads.end();)
	{
		if(detail::thread_impl_interface* inUseThread = itInUseThread->second)
		{
			m_mutex.unlock();

			inUseThread->stop().dismiss();

			m_mutex.lock();
		}

		//after retorning from stop we can ensure itInUseThread has been deleted from m_underUseThreads
		itInUseThread = m_underUseThreads.begin();
	}

	m_condVar.wait_until(m_mutex,[this](){ return m_underUseThreads.empty() == false; });

	thread_container::iterator itThread = m_availableThreads.begin();
	for(;itThread!= m_availableThreads.end();++itThread)
	{
		delete(*itThread);
	}

	m_mutex.unlock();
}
size_t thread_pool::set_affinity(const cpu_set_t& i_set)
{
	size_t res = 0;

	mutex_guard lg(m_mutex);
	thread_container::iterator itThread = m_availableThreads.begin();
	for(; itThread != m_availableThreads.end(); ++itThread)
	{
		if(detail::thread_impl_interface* threadImpl = *itThread)
		{
			if(threadImpl->set_affinity(i_set))
			{
				++res;
			}
		}
	}

	return res;
}
thread_pool::acquire_result<thread> thread_pool::aquire_thread()
{
	mutex_guard lg(m_mutex);

	if(m_availableThreads.empty())
	{
		if(m_policy == GrowsOnDemand)
		{
			detail::thread_impl_interface* acquiredThread = new detail::worker_thread_impl();

			m_underUseThreads[static_cast<const void*>(acquiredThread)] = acquiredThread;

			return make_result<acquire_result<thread>>(as_unique_reference(acquiredThread,deallocator_proxy{ *this }));
		}
		else
		{
			return make_error<acquire_result<thread>>(NoThreadAvailable);
		}
	}
	else
	{
		thread_container::iterator itThread = m_availableThreads.begin() + m_availableThreads.size() - 1;

		detail::thread_impl_interface* acquiredThread = *itThread;

		m_availableThreads.erase(itThread);

		m_underUseThreads[static_cast<const void*>(acquiredThread)] = acquiredThread;

		return make_result<acquire_result<thread>>(as_unique_reference(acquiredThread,deallocator_proxy{ *this }));
	}
}
thread_pool::acquire_result<thread_sheaf> thread_pool::acquire_sheaf(size_t i_size)
{
	mutex_guard lg(m_mutex);

	const size_t missingThreads = (m_availableThreads.size() < i_size) ? i_size - m_availableThreads.size() : 0;
	if(missingThreads == 0 || m_policy == GrowsOnDemand && m_underUseThreads.size() < (m_maxNumThreads - missingThreads))
	{
		const size_t availableThreads = i_size - missingThreads;
		thread_sheaf threadSheaf;
		threadSheaf.m_threadCtr.reserve(i_size);
		m_underUseThreads.reserve(m_underUseThreads.size() + i_size);

		thread_container::iterator itThread = m_availableThreads.begin();
		for(size_t threadIndex = 0; threadIndex < availableThreads; ++threadIndex,++itThread)
		{
			detail::thread_impl_interface* acquiredThread = *itThread;

			threadSheaf.m_threadCtr.push_back(as_unique_reference(acquiredThread,deallocator_proxy{ *this }));

			m_underUseThreads[static_cast<const void*>(acquiredThread)] = acquiredThread;
		}
		m_availableThreads.erase(m_availableThreads.begin(),m_availableThreads.begin() + availableThreads);

		for(size_t threadIndex = 0; threadIndex < missingThreads; ++threadIndex)
		{
			detail::thread_impl_interface* newThread = new detail::worker_thread_impl();

			m_underUseThreads[static_cast<const void*>(newThread)] = newThread;

			threadSheaf.m_threadCtr.push_back(as_unique_reference(newThread,deallocator_proxy{ *this }));
		}

		return make_result<acquire_result<thread_sheaf>>(std::move(threadSheaf));
	}
	else
	{
		return make_error<acquire_result<thread_sheaf>>(NoThreadAvailable);
	}
}
bool thread_pool::available_threads() const
{
	mutex_guard lg(m_mutex);

	return m_availableThreads.empty() == false;
}
void thread_pool::deallocate(detail::thread_impl_interface* i_object) const
{
	bool awareAboutNewThreads = false;

	{
		mutex_guard lg(m_mutex);

		thread_in_use_container::iterator itThread = m_underUseThreads.find(i_object);
		for(;itThread!=m_underUseThreads.end();++itThread)
		{
			if(itThread->first == i_object)
			{
				m_availableThreads.push_back(itThread->second);

				m_underUseThreads.erase(itThread);

				awareAboutNewThreads = true;

				m_condVar.notify_one();

				break;
			}
		}
	}

	if(awareAboutNewThreads)
	{
		on_availableThreads.execute();
	}
}

}
