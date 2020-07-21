#pragma once

#include "ddk_ucontext.h"
#include "ddk_reference_wrapper.h"
#include "ddk_thread_impl.h"

namespace ddk
{
namespace detail
{

template<typename Comparator>
bool priority_queue<Comparator>::has_item(const fiber_id& i_id) const
{
	return std::find(this->c.begin(), this->c.end(),i_id) != this->c.end();
}

}

template<typename Comparator>
fiber_scheduler<Comparator>::fiber_scheduler()
: m_yielder(static_cast<detail::fiber_yielder_interface&>(*this))
, m_stop(false)
{
	pthread_mutex_init(&m_fiberMutex, NULL);
	pthread_cond_init(&m_fiberCondVar, NULL);
}
template<typename Comparator>
fiber_scheduler<Comparator>::~fiber_scheduler()
{
	DDK_ASSERT(m_fibers.empty(), "Destroying fiber scheduler with pending fibers");

	stop();

	pthread_cond_destroy(&m_fiberCondVar);
	pthread_mutex_destroy(&m_fiberMutex);
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::register_fiber_result fiber_scheduler<Comparator>::register_fiber(fiber& i_fiber)
{
	pthread_mutex_lock(&m_fiberMutex);

	fiber_container::const_iterator itFiber = m_fibers.find(i_fiber.get_id());

	if(itFiber == m_fibers.end())
	{
		i_fiber.m_impl->set_executor(this->template ref_from_this<detail::fiber_scheduler_interface>(*this));

		std::pair<fiber_container::const_iterator,bool> insertRes = m_fibers.insert(std::make_pair(i_fiber.get_id(),i_fiber.m_impl.get()));

		pthread_mutex_unlock(&m_fiberMutex);

		if(insertRes.second)
		{
			return success;
		}
		else
		{
			return make_error<register_fiber_result>(InternalError);
		}
	}
	else
	{
		pthread_mutex_unlock(&m_fiberMutex);

		return make_error<register_fiber_result>(FiberAlreadyRegistered);
	}
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::unregister_fiber_result fiber_scheduler<Comparator>::unregister_fiber(fiber_id i_id)
{
	pthread_mutex_lock(&m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);

	if(itFiber != m_fibers.end())
	{
		itFiber->second->set_executor(nullptr);

		m_fibers.erase(itFiber);

		pthread_mutex_unlock(&m_fiberMutex);

		return success;
	}
	else
	{
		pthread_mutex_unlock(&m_fiberMutex);

		return make_error<unregister_fiber_result>(UnexistentFiber);
	}
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::iterator fiber_scheduler<Comparator>::begin()
{
	return m_fibers.begin();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::iterator fiber_scheduler<Comparator>::end()
{
	return m_fibers.end();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::const_iterator fiber_scheduler<Comparator>::begin() const
{
	return m_fibers.begin();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::const_iterator fiber_scheduler<Comparator>::end() const
{
	return m_fibers.end();
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::empty() const
{
	return m_fibers.empty();
}
template<typename Comparator>
size_t fiber_scheduler<Comparator>::size() const
{
	return m_fibers.size();
}
template<typename Comparator>
void fiber_scheduler<Comparator>::start()
{
	m_fiberThread.start(std::bind(&fiber_scheduler<Comparator>::run,this),ddk::lend(m_yielder));
}
template<typename Comparator>
void fiber_scheduler<Comparator>::stop()
{
	if(m_fiberThread.joinable())
	{
		//cleanup
		pthread_mutex_lock(&m_fiberMutex);

		m_functions.clear();

		while (m_runningFibers.empty() == false)
		{
			detail::running_fiber topFiber = m_runningFibers.top();

			pthread_mutex_unlock(&m_fiberMutex);

			topFiber->stop();

			pthread_mutex_lock(&m_fiberMutex);
		}

		m_stop = true;

		pthread_cond_signal(&m_fiberCondVar);

		pthread_mutex_unlock(&m_fiberMutex);

		m_fiberThread.stop();
	}
}
template<typename Comparator>
void fiber_scheduler<Comparator>::yield(detail::yielder_context* i_context)
{
	m_callee.set_last_time(time(NULL));

	m_runningFibers.push(++m_callee);

	m_callee->resume_to(m_caller);
}
template<typename Comparator>
void fiber_scheduler<Comparator>::suspend(detail::yielder_context* i_context)
{
	pthread_cond_signal(&m_fiberCondVar);

	throw detail::suspend_exception(m_callee->get_id());
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::activate(fiber_id i_id, const std::function<void()>& i_function)
{
	pthread_mutex_lock(&m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);
	if(itFiber != m_fibers.end())
	{
		std::pair<function_container::iterator,bool> insertRes = m_functions.insert(std::make_pair(i_id,i_function));

		pthread_cond_signal(&m_fiberCondVar);

		pthread_mutex_unlock(&m_fiberMutex);

		return true;
	}
	else
	{
		DDK_FAIL("Trying to activate not registered fiber");

		pthread_mutex_unlock(&m_fiberMutex);

		return false;
	}
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::deactivate(fiber_id i_id)
{
	pthread_mutex_lock(&m_fiberMutex);

	function_container::iterator itFunction = m_functions.find(i_id);
	if(itFunction != m_functions.end())
	{
		m_functions.erase(itFunction);

		pthread_mutex_unlock(&m_fiberMutex);

		return true;
	}
	else
	{
		while (m_runningFibers.has_item(i_id))
		{
			pthread_cond_wait(&m_fiberCondVar,&m_fiberMutex);
		}

		pthread_mutex_unlock(&m_fiberMutex);

		return false;
	}
}
template<typename Comparator>
void fiber_scheduler<Comparator>::unregister(fiber_id i_id)
{
	pthread_mutex_lock(&m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);

	if(itFiber != m_fibers.end())
	{
		m_fibers.erase(itFiber);
	}
	else
	{
		DDK_FAIL("Trying to unregister not present fiber");
	}

	pthread_mutex_unlock(&m_fiberMutex);
}
template<typename Comparator>
void fiber_scheduler<Comparator>::run()
{
	//update current yielder
	detail::yielder_lent_ptr prevYielder = detail::thread_impl_interface::set_yielder(ddk::lend(m_yielder));

	while(m_stop == false)
	{
		std::function<void()> callableObject;

		pthread_mutex_lock(&m_fiberMutex);

		function_container::iterator itFunction = m_functions.begin();
		if(itFunction != m_functions.end())
		{
			fiber_container::iterator itFiber = m_fibers.find(itFunction->first);

			if(detail::fiber_impl* currFiber = itFiber->second)
			{
				callableObject = itFunction->second;

				m_functions.erase(itFunction);

				pthread_mutex_unlock(&m_fiberMutex);

				currFiber->start_from(m_caller,callableObject);

				m_runningFibers.emplace(currFiber,0);
			}
		}
		else
		{
			pthread_mutex_unlock(&m_fiberMutex);
		}

		if(m_runningFibers.empty() == false)
		{
			m_callee = m_runningFibers.top();

			m_runningFibers.pop();

			m_callee->resume_from(m_caller);
		}
		else
		{
			pthread_mutex_lock(&m_fiberMutex);

			if (m_stop == false)
			{
				pthread_cond_wait(&m_fiberCondVar,&m_fiberMutex);
			}

			pthread_mutex_unlock(&m_fiberMutex);
		}
	}

	//recover previous yielder
	detail::thread_impl_interface::set_yielder(prevYielder);
}

}
