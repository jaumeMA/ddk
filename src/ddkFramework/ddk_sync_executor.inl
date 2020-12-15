
namespace ddk
{

template<typename Return>
shared_reference_wrapper<async_executor<Return>> make_async_executor(const function<Return()>& i_function)
{
	return make_shared_reference<async_executor<Return>>(i_function);
}

template<typename Return>
async_executor<Return>::async_executor(const function<Return()>& i_function)
: m_function(i_function)
, m_executor(make_executor<detail::deferred_executor<Return>>())
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
	notify();
}
template<typename Return>
future<Return> async_executor<Return>::attach(thread i_thread)
{
	m_executor = make_executor<detail::thread_executor<Return>>(std::move(i_thread));

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(fiber i_fiber)
{
	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(thread_sheaf i_threadSheaf)
{
	//at some point put a composed callable here
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_promise.detach();
	m_executor.clear();

	start_result execRes = newAsyncExecutor->execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(fiber_sheaf i_fiberSheaf)
{
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_promise.detach();
	m_executor.clear();

	start_result execRes = newAsyncExecutor->execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(attachable<Return> i_attachable)
{
	m_executor = std::move(i_attachable.m_executorImpl);

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(thread i_thread)
{
	m_executor = make_executor<detail::thread_executor<Return>>(std::move(i_thread));

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(fiber i_fiber)
{
	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(thread_sheaf i_threadSheaf)
{
	//at some point put a composed callable here
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_promise.detach();
	m_executor.clear();

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(fiber_sheaf i_fiberSheaf)
{
	async_executor<detail::void_t>* newAsyncExecutor = new async_executor<detail::void_t>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_promise.detach();
	m_executor.clear();

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(attachable<Return> i_attachable)
{
	m_executor = std::move(i_attachable.m_executorImpl);

	return as_future();
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::store(promise<Return>& i_promise)
{
	m_promise = i_promise;

	return this->ref_from_this();
}
template<typename Return>
typename async_executor<Return>::async_shared_ref async_executor<Return>::on_cancel(const ddk::function<bool()>& i_cancelFunc)
{
	m_cancelFunc = i_cancelFunc;

	return this->ref_from_this();
}
template<typename Return>
typename async_executor<Return>::start_result async_executor<Return>::execute()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to execute empty executor"};
    }

	const nested_start_result execRes = m_executor->execute(make_function(this,&async_executor<Return>::set_value),
	make_function([self = this->unsafe_ref_from_this()]() mutable
	{
		try
		{
			return eval(self->m_function);
		}
		catch(const async_exception& i_excp)
		{
			self->set_exception(i_excp);

			throw;
		}
		catch(const std::exception& i_excp)
		{
			self->set_exception(async_exception{ i_excp.what() });

			throw;
		}
		catch(...)
		{
			self->set_exception(async_exception{ "Unkwon exception" });

			throw;
		}
	}));

	if(execRes == success)
	{
		const ExecutorState currState = execRes.get();

		if(currState != ExecutorState::Idle)
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
	m_promise.set_value(i_value);
}
template<typename Return>
void async_executor<Return>::set_exception(const async_exception& i_excp)
{
	m_promise.set_exception(i_excp);
}
template<typename Return>
future<Return> async_executor<Return>::as_future()
{
	m_promise.attach(this->ref_from_this());

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
		start_result execRes = execute();

		DDK_ASSERT(execRes == success,"Error while trying to execute async executor");

		return true;
	}
	else
	{
		return false;
	}
}

}
