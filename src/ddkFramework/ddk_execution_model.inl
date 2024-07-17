
namespace ddk
{
namespace detail
{

template<typename Event>
template<typename Callable>
auto signaled_execution_model<Event>::instantiate(Callable&& i_callable)
{
	return execution<Callable>{ std::forward<Callable>(i_callable),*this };
}
template<typename Event>
void signaled_execution_model<Event>::signal(Event i_event)
{
	if (m_signaler != nullptr)
	{
		eval(m_signaler,std::move(i_event));
	}
}
template<typename Event>
void signaled_execution_model<Event>::resume()
{
	m_signaler = nullptr;
}

template<typename Event>
template<typename Callable>
TEMPLATE(typename CCallable)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable))
signaled_execution_model<Event>::execution<Callable>::execution(CCallable&& i_callable,signaled_execution_model& i_model)
: m_callable(std::forward<CCallable>(i_callable))
{
	i_model.m_signaler = ddk::make_function(m_callable);
}
template<typename Event>
template<typename Callable>
auto signaled_execution_model<Event>::execution<Callable>::operator()()
{
}

template<typename Callable>
auto polling_execution_model::instantiate(Callable&& i_callable)
{
	return execution<Callable>{ std::forward<Callable>(i_callable),*this };
}

template<typename Callable>
TEMPLATE(typename CCallable)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable))
polling_execution_model::execution<Callable>::execution(CCallable&& i_callable, polling_execution_model& i_model)
: m_callable(std::forward<CCallable>(i_callable))
, m_model(i_model)
{
	m_model.m_stopped = false;
}
template<typename Callable>
auto polling_execution_model::execution<Callable>::operator()()
{
	std::chrono::milliseconds systemDelta = std::chrono::milliseconds(0);

	while (m_model.m_stopped == false)
	{
		const std::chrono::steady_clock::time_point beforeEval = std::chrono::steady_clock::now();

		ddk::eval(m_callable);

		const std::chrono::steady_clock::time_point afterEval = std::chrono::steady_clock::now();
		const std::chrono::milliseconds evalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(afterEval - beforeEval);

		if (evalDuration < (m_model.m_sleepTimeInMS - systemDelta))
		{
			const std::chrono::milliseconds remainingWaitingTime = m_model.m_sleepTimeInMS - evalDuration;

			const std::chrono::steady_clock::time_point beforeSleep = std::chrono::steady_clock::now();

			std::this_thread::sleep_for(remainingWaitingTime - systemDelta);

			const std::chrono::steady_clock::time_point afterSleep = std::chrono::steady_clock::now();

			systemDelta += (std::chrono::duration_cast<std::chrono::milliseconds>(afterSleep - beforeSleep) - remainingWaitingTime) / 2;
		}
		else
		{
			systemDelta = std::chrono::milliseconds(0);

			std::this_thread::yield();
		}
	}
}

template<typename Callable>
auto async_execution_model::instantiate(Callable&& i_callable)
{
	return instantiate(std::forward<Callable>(i_callable),[]() { return false; });
}
template<typename Callable,typename CCallable>
auto async_execution_model::instantiate(Callable&& i_callable,CCallable&& i_test)
{
	return execution<Callable,CCallable>{ std::forward<Callable>(i_callable),std::forward<CCallable>(i_test),*this };
}

template<typename Callable,typename CCallable>
TEMPLATE(typename CCCallable,typename CCCCallable)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCCallable),IS_CONSTRUCTIBLE(CCallable,CCCCallable))
async_execution_model::execution<Callable,CCallable>::execution(CCCallable&& i_callable,CCCCallable&& i_test, async_execution_model& i_model)
: m_callable(std::forward<CCCallable>(i_callable))
, m_test(i_test)
, m_model(i_model)
{
	m_model.m_stopped = false;
}
template<typename Callable,typename CCallable>
auto async_execution_model::execution<Callable,CCallable>::operator()()
{
	m_model.m_condVarMutex.lock();

	while (m_model.m_stopped == false || m_model.m_pendingWork)
	{
		m_model.m_pendingWork = false;

		m_model.m_condVarMutex.unlock();

		ddk::eval(m_callable);

		m_model.m_condVarMutex.lock();

		m_model.m_condVar.wait_until(m_model.m_condVarMutex,[&]() { mutex_guard mg(m_model.m_condVarMutex); return (ddk::eval(m_test) || m_model.m_pendingWork || m_model.m_stopped) == false; },m_model.m_sleepTimeInMS);
	}

	m_model.m_condVarMutex.unlock();
}

template<typename Callable>
auto forward_execution_model::instantiate(Callable&& i_callable)
{
	return execution<Callable>{std::forward<Callable>(i_callable)};
}

template<typename Callable>
TEMPLATE(typename CCallable)
REQUIRED(IS_CONSTRUCTIBLE(Callable,CCallable))
forward_execution_model::execution<Callable>::execution(CCallable&& i_callable)
: m_callable(std::forward<Callable>(i_callable))
{
}
template<typename Callable>
auto forward_execution_model::execution<Callable>::operator()()
{
	ddk::eval(m_callable);
}

}
}