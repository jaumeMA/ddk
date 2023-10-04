
namespace ddk
{

template<typename T>
future<future<T>>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T>
future<future<T>>::future(const detail::private_async_state_shared_ptr<future<T>>& i_sharedState, unsigned char i_depth)
: m_sharedState(i_sharedState)
, m_depth(i_depth)
{
}
template<typename T>
future<future<T>>::future(detail::private_async_state_shared_ptr<future<T>>&& i_sharedState, unsigned char i_depth)
: m_sharedState(std::move(i_sharedState))
, m_depth(i_depth)
{
}
template<typename T>
future<future<T>>& future<future<T>>::operator=(future<future<T>>&& other)
{
	m_sharedState = std::move(other.m_sharedState);

	return *this;
}
template<typename T>
bool future<future<T>>::valid() const
{
	return m_sharedState != nullptr;
}
template<typename T>
bool future<future<T>>::ready() const
{
	if(m_sharedState)
	{
		return m_sharedState->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
bool future<future<T>>::empty() const
{
	return m_sharedState == nullptr;
}
template<typename T>
typename future<future<T>>::const_reference future<future<T>>::get_value() const
{
	if (m_sharedState)
	{
		return m_sharedState->get_value();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
auto future<future<T>>::extract_value() &&
{
	if(m_sharedState)
	{
		return std::move(*m_sharedState).extract_value().extract();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
typename future<future<T>>::cancel_result future<future<T>>::cancel()
{
	if(m_sharedState)
	{
		const auto cancelRes = m_sharedState->cancel();

		if(cancelRes)
		{
			m_sharedState = nullptr;
		}

		return cancelRes;
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
void future<future<T>>::wait() const
{
	if(m_sharedState)
	{
		return m_sharedState->wait();
	}
}
template<typename T>
bool future<future<T>>::wait_for(const std::chrono::milliseconds& i_period) const
{
	if(m_sharedState)
	{
		m_sharedState->wait_for(i_period);

		return m_sharedState->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,nested_rreference))
auto future<future<T>>::then(Callable&& i_continuation) &&
{
	if (detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (executor_context_lent_ptr context = asyncExecutor->get_execution_context())
			{
				//here we capture as well asyncExecutor so we ensure original future lifetime exceeds the one of i_future
				auto continuation = [context,asyncExecutor,i_continuation](future<T> i_future) mutable
				{
					return std::move(i_future).chain(i_continuation,promote_to_ref(context));
				};

				return contraction(std::move(*this)._async(continuation,context));
			}
		}

		//if no context available, future should be ready so this is a regular then
		return std::move(*this).extract_value().then(std::forward<Callable>(i_continuation));
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
future<T> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	if(detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = ddk::async([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(*std::move(*acquiredFuture.m_sharedState).extract_value()).extract_value();
				}
				else
				{
					return std::move(*std::move(*acquiredFuture.m_sharedState).extract_value()).extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		});

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			future<T> res = executor -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return std::move(executor);
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&
{
	if(detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		future<T> res = ddk::async([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(*std::move(*acquiredFuture.m_sharedState).extract_value()).extract_value();
				}
				else
				{
					return std::move(*std::move(*acquiredFuture.m_sharedState).extract_value()).extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}) -> attach(i_execContext,currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<future<T>>::_then(Callable&& i_continuation) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if (detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		auto executor = ddk::async([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr (mpl::is_void<return_type>)
			{
				eval(i_continuation,std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation,std::move(acquiredFuture).extract_value());
			}
		});

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			const unsigned char currDepth = m_depth;

			return_type res = std::move(executor)->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return return_type{ std::move(executor) };
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto future<future<T>>::_async(Callable&& i_continuation, executor_context_lent_ptr i_execContext) &&
{
	typedef future<typename mpl::aqcuire_callable_return_type<Callable>::type> return_type;

	if (detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		return_type res = ddk::async([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr (mpl::is_void<return_type>)
			{
				eval(i_continuation,std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation,std::move(acquiredFuture).extract_value());
			}
		}) -> attach(i_execContext,currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}

}
