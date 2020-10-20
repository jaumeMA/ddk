
namespace ddk
{

template<typename Return>
shared_reference_wrapper<async_executor<Return>> make_async_executor(const function<Return()>& i_function)
{
	async_executor<Return>* newAsyncExecutor = new async_executor<Return>(i_function);

	return as_shared_reference(newAsyncExecutor,tagged_pointer<shared_reference_counter>(&newAsyncExecutor->m_refCounter,ReferenceAllocationType::Embedded));
}

template<typename Return>
async_executor<Return>::async_executor(const function<Return()>& i_function)
: m_executor(make_executor<detail::deferred_executor<Return>>())
, m_sharedState(make_shared_reference<detail::private_async_state<Return>>())
, m_function(i_function)
{
}
template<typename Return>
async_executor<Return>::async_executor(async_executor&& other)
: m_function(std::move(other.m_function))
, m_executor(std::move(other.m_executor))
, m_sharedState(std::move(other.m_sharedState))
{
}
template<typename Return>
async_executor<Return>::~async_executor()
{
	//if not executed, excute and wait for its result
	if(m_executor && m_executor->get_state() == ExecutorState::Idle)
	{
        execute();

        m_sharedState->wait();
	}
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(thread i_thread)
{
	DDK_ASSERT(i_thread.get_id() != ddk::get_current_thread_id(), "Trying to launch async over operation over calle thread");

	m_executor = make_executor<detail::thread_executor<Return>>(std::move(i_thread));

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(const detail::this_thread_t&)
{
	m_executor = make_executor<detail::deferred_executor<Return>>();

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(fiber i_fiber)
{
	DDK_ASSERT(i_fiber.get_id() == ddk::get_current_fiber_id(), "Trying to launch async over operation over calle fiber");

	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(const detail::this_fiber_t& i_fiber)
{
	m_executor = make_executor<detail::await_executor<Return>>(i_fiber.get_allocator());

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
shared_reference_wrapper<async_executor<detail::void_t>> async_executor<Return>::attach(thread_sheaf i_threadSheaf)
{
	const ddk::function<Return()> thisFunction(m_function);

	//at some point put a composed callable here
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(ddk::function<detail::void_t()>([thisFunction]() -> detail::void_t { thisFunction(); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_sharedState.clear();
	m_executor.clear();

	return as_shared_reference(newAsyncExecutor,tagged_pointer<shared_reference_counter>(&(newAsyncExecutor->m_refCounter),ReferenceAllocationType::Embedded));
}
template<typename Return>
shared_reference_wrapper<async_executor<detail::void_t>> async_executor<Return>::attach(fiber_sheaf i_fiberSheaf)
{
	const ddk::function<Return()> thisFunction(m_function);

	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(ddk::function<detail::void_t()>([thisFunction]() -> detail::void_t { thisFunction(); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_sharedState.clear();
	m_executor.clear();

	return as_shared_reference(newAsyncExecutor,tagged_pointer<shared_reference_counter>(&(newAsyncExecutor->m_refCounter),ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(attachable<Return> i_attachable)
{
	m_executor = std::move(i_attachable.m_executorImpl);

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::store(promise<Return>& i_promise)
{
	async_shared_ref thisRef = as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));

	m_sharedState = i_promise = thisRef;

	return thisRef;
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::on_cancel(const ddk::function<bool()>& i_cancelFunc)
{
	m_cancelFunc = i_cancelFunc;

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
typename async_executor<Return>::start_result async_executor<Return>::execute()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to execute empty executor"};
    }

	const nested_start_result execRes = m_executor->execute(ddk::make_function(this,&async_executor<Return>::set_value),m_function);

	if(execRes == success)
	{
		const ExecutorState currState = execRes.get();

		if(currState == ExecutorState::Executed)
		{
			return make_error<start_result>(StartErrorCode::AlreadyDone);
		}
		else
		{
			return success;
		}
	}
	else
	{
		return make_error<start_result>(StartErrorCode::NotAvailable);
	}
}
template<typename Return>
void async_executor<Return>::set_value(Return i_value)
{
	m_sharedState->set_value(i_value);
}
template<typename Return>
void async_executor<Return>::bind()
{
	nested_start_result execRes = m_executor->execute(ddk::make_function(this,&async_executor<Return>::set_value),m_function);

	DDK_ASSERT(execRes == success, "Error while binding async executor");
}
template<typename Return>
typename async_executor<Return>::reference async_executor<Return>::get_value()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to get value from empty executor"};
    }

	if(m_executor->get_state() == ExecutorState::Idle)
	{
        throw async_exception{"Trying to get value from idled executor"};
	}

	return m_sharedState->get_value();
}
template<typename Return>
typename async_executor<Return>::cancel_result async_executor<Return>::cancel()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to cancel from empty executor"};
    }

	const cancel_result cancelRes = m_executor->cancel(m_cancelFunc);

	if (cancelRes == success)
	{
		m_sharedState->signal();
	}

	return cancelRes;
}
template<typename Return>
typename async_executor<Return>::const_reference async_executor<Return>::get_value() const
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to get value from empty executor"};
    }

	if(m_executor->get_state() == ExecutorState::Idle)
	{
        throw async_exception{"Trying to get value from idled executor"};
	}

	return m_sharedState->get_value();
}
template<typename Return>
typename async_executor<Return>::rref_type async_executor<Return>::extract_value()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to extract value from empty executor"};
    }

	if(m_executor->get_state() == ExecutorState::Idle)
	{
        throw async_exception{"Trying to extract value from idled executor"};
	}

	return m_sharedState->extract_value();
}
template<typename Return>
bool async_executor<Return>::ready() const
{
	return m_sharedState->ready();
}
template<typename Return>
void async_executor<Return>::wait() const
{
	m_sharedState->wait();
}
template<typename Return>
void async_executor<Return>::wait_for(unsigned int i_period) const
{
	m_sharedState->wait_for(i_period);
}

}
