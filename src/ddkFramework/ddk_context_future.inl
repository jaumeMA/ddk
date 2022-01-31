
namespace ddk
{

template<typename Return>
TEMPLATE(typename T)
REQUIRED(IS_CONSTRUCTIBLE(future<Return>,T))
context_future<Return>::context_future(T&& i_future)
: future<Return>(std::forward<T>(i_future))
{
}
template<typename Return>
context_future<Return>::context_future(future<Return> i_future, promised_context_dist_ref i_context)
: future<Return>(std::move(i_future))
, m_context(i_context)
{
}
template<typename Return>
context_future<Return>::context_future(future<Return>&& i_future)
: future<Return>(std::move(i_future))
{
}
template<typename Return>
template<typename T>
context_future<T> context_future<Return>::then(const ddk::function<T(Return)>& i_callback) &&
{
	if(promised_context_dist_ptr context = std::move(m_context))
	{
		return { std::move(*this).future<Return>::async(i_callback,ddk::static_lent_cast<executor_context_interface>(ddk::lend(context))),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		return std::move(*this).future<Return>::then(i_callback);
	}
}
template<typename Return>
context_future<Return> context_future<Return>::on_error(const function<void(const async_error&)>& i_onError) &&
{
	if(promised_context_dist_ptr context = std::move(m_context))
	{
		return { std::move(*this).future<Return>::on_error(i_onError,ddk::static_lent_cast<executor_context_interface>(ddk::lend(context))),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		return std::move(*this).future<Return>::on_error(i_onError);
	}
}

}