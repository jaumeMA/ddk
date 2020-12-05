
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
, m_function(i_function)
{
}
template<typename Return>
async_executor<Return>::async_executor(async_executor&& other)
: m_function(std::move(other.m_function))
, m_executor(std::move(other.m_executor))
, m_promise(std::move(other.m_promise))
{
}
template<typename Return>
async_executor<Return>::~async_executor()
{
	//if not executed, excute and wait for its result
	if(notify())
	{
        m_promise.wait();
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
typename async_executor<Return>::async_shared_ref async_executor<Return>::attach(fiber i_fiber)
{
	DDK_ASSERT(i_fiber.get_id() == ddk::get_current_fiber_id(), "Trying to launch async over operation over calle fiber");

	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));
}
template<typename Return>
shared_reference_wrapper<async_executor<detail::void_t>> async_executor<Return>::attach(thread_sheaf i_threadSheaf)
{
	//at some point put a composed callable here
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_promise.detach();
	m_executor.clear();

	return as_shared_reference(newAsyncExecutor,tagged_pointer<shared_reference_counter>(&(newAsyncExecutor->m_refCounter),ReferenceAllocationType::Embedded));
}
template<typename Return>
shared_reference_wrapper<async_executor<detail::void_t>> async_executor<Return>::attach(fiber_sheaf i_fiberSheaf)
{
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_promise.detach();
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
	m_promise = i_promise;

	return as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded));;
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

		if(currState == ExecutorState::Executing)
		{
			return success;
		}
		else
		{
			return make_error<start_result>(StartErrorCode::NotAvailable);
		}
	}
	else
	{
		return make_error<start_result>(StartErrorCode::NotAvailable);
	}
}
template<typename Return>
void async_executor<Return>::set_value(sink_reference i_value)
{
	m_executor = nullptr;

	m_promise.set_value(i_value);
}
template<typename Return>
future<Return> async_executor<Return>::as_future()
{
	m_promise.attach(as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded)));

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone, "Trying to execute an alerady executed async executor");

	return m_promise.get_future();
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
		m_promise.signal();
	}

	return cancelRes;
}
template<typename Return>
bool async_executor<Return>::notify()
{
	//if not executed, excute
	if(m_executor && m_executor->get_state() == ExecutorState::Idle)
	{
		if(execute() != success)
		{
			// if still not executed, force an execution from this very context
			set_value(eval(m_function));
		}

		return true;
	}
	else
	{
		return false;
	}
}

}
