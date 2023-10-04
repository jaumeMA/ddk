
namespace ddk
{

template<typename T>
context_future<context_future<T>>::context_future(context_future<context_future<T>>&& other)
	: future<context_future<T>>(std::move(other))
	,m_context(std::move(other.m_context))
{
}
template<typename T>
context_future<context_future<T>>::context_future(future<context_future<T>>&& i_future)
	: future<context_future<T>>(std::move(i_future))
{
}
template<typename T>
context_future<context_future<T>>::context_future(future<context_future<T>>&& i_future,promised_context_dist_ref i_context)
	: future<context_future<T>>(std::move(i_future))
	,m_context(i_context)
{
}
template<typename T>
TEMPLATE(typename Return,typename Type)
REQUIRED(IS_CONSTRUCTIBLE(Type,rreference))
context_future<Return> context_future<context_future<T>>::then(const function<Return(Type)>& i_continuation)&&
{
	if (promised_context_dist_ptr context = std::move(m_context))
	{
		auto continuation = make_function([context,i_continuation](context_future<T> i_future) mutable
			{
				i_future.m_context->transfer_recipients(std::move(*context));

				return std::move(i_future).chain(i_continuation);
			});

		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return contraction(context_future<context_future<Return>>{ std::move(*this).future<context_future<T>>::async(continuation,execContext),std::move(ddk::promote_to_ref(context)) });
	}
	else
	{
		throw ddk::async_exception{ "You cannot continue context_futures without context" };
	}
}
template<typename T>
template<typename Return,typename Type>
context_future<Return> context_future<context_future<T>>::then(const function<Return(context_future<Type>)>& i_continuation)&&
{
	if (promised_context_dist_ptr context = std::move(m_context))
	{
		auto continuation = make_function([context,i_continuation](context_future<T> i_future) mutable
			{
				i_future.m_context->transfer_recipients(std::move(*context));

				return ddk::eval(i_continuation,std::move(i_future));
			});

		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(context));

		return { std::move(*this).future<context_future<T>>::async(continuation,execContext),std::move(ddk::promote_to_ref(context)) };
	}
	else
	{
		throw ddk::async_exception{ "You cannot continue context_futures without context" };
	}
}
template<typename T>
context_future<T> context_future<context_future<T>>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	if (promised_context_dist_ptr context = std::move(m_context))
	{
		auto onError = make_function([i_onError](context_future<T> i_future) mutable
			{
				return std::move(i_future).on_error(i_onError);
			});

		executor_context_lent_ptr execContext = ddk::static_lent_cast<executor_context_interface>(ddk::lend(m_context));

		return contraction(context_future<context_future<Return>>{ std::move(*this).future<context_future<T>>::on_error(onError,execContext),std::move(ddk::promote_to_ref(m_context)) });
	}
	else
	{
		throw ddk::async_exception{ "You cannot continue context_futures without context" };
	}
}

}