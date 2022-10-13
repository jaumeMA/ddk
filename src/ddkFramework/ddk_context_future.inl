
namespace ddk
{

template<typename Return>
context_future<Return>::context_future(context_future<Return>&& other)
: future<Return>(std::move(other))
, m_context(std::move(other.m_context))
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
context_future<Return>::context_future()
{
}
template<typename Return>
template<typename T>
context_future<T> context_future<Return>::then(const ddk::function<T(Return)>& i_callback) &&
{
	if(promised_context_dist_ptr context = std::move(m_context))
	{
		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return { std::move(*this).future<Return>::async(i_callback,execContext),std::move(ddk::promote_to_ref(context)) };
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
		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return { std::move(*this).future<Return>::on_error(i_onError,execContext),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		return std::move(*this).future<Return>::on_error(i_onError);
	}
}

}