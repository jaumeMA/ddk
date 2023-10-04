
namespace ddk
{

template<typename T>
context_future<T>::context_future(context_future<T>&& other)
: future<T>(std::move(other))
, m_context(std::move(other.m_context))
{
}
template<typename T>
context_future<T>::context_future(future<T>&& i_future)
: future<T>(std::move(i_future))
{
}
template<typename T>
context_future<T>::context_future(future<T>&& i_future,promised_context_dist_ref i_context)
: future<T>(std::move(i_future))
, m_context(i_context)
{
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto context_future<T>::then(Callable&& i_callback)&&
{
	typedef context_future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;
	
	if (promised_context_dist_ptr context = std::move(m_context))
	{
		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return return_type{ std::move(*this).future<T>::async(i_callback,execContext),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		return return_type{ std::move(*this).future<T>::then(i_callback) };
	}
}
template<typename T>
context_future<T> context_future<T>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	if (promised_context_dist_ptr context = std::move(m_context))
	{
		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return { std::move(*this).future<T>::on_error(i_onError,execContext),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		return std::move(*this).future<T>::on_error(i_onError);
	}
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto context_future<T>::chain(Callable&& i_callback) &&
{
	typedef context_future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if (promised_context_dist_ptr context = std::move(m_context))
	{
		if constexpr (is_context_future<Return>::value)
		{
			auto continuation = [i_callback,context](Type i_value) mutable -> Return
			{
				Return res = ddk::eval(i_callback,std::move(i_value));

				res.m_context->transfer_recipients(std::move(*context));

				return std::move(res);
			};

			executor_context_lent_ptr execContext = static_lent_cast<executor_context_interface>(ddk::lend(context));

			return return_type{ std::move(*this).future<T>::async(make_function(continuation),execContext),std::move(ddk::promote_to_ref(context)) };
		}
		else
		{
			executor_context_lent_ptr execContext = static_lent_cast<executor_context_interface>(ddk::lend(context));

			return return_type{ std::move(*this).future<T>::async(i_callback,execContext),std::move(ddk::promote_to_ref(context)) };
		}

	}
	else
	{
		return return_type{ std::move(*this).future<T>::then(i_callback) };
	}
}

}

#include "ddk_context_future_future.inl"