
namespace ddk
{

template<typename T>
future<future<T>>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T>
future<future<T>>::future(const detail::private_async_state_dist_ptr<future<T>>& i_sharedState)
: m_sharedState(i_sharedState)
{
}
template<typename T>
future<future<T>>::future(detail::private_async_state_dist_ptr<future<T>>&& i_sharedState)
: m_sharedState(std::move(i_sharedState))
{
}
template<typename T>
template<typename TT>
future<future<T>>::future(distributed_reference_wrapper<future<TT>> i_executor,...)
: future(i_executor->as_future())
{
}
template<typename T>
future<future<T>>::~future()
{
	if(m_sharedState)
	{
		m_sharedState->notify();
	}
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
const future<T>& future<future<T>>::get_future() const
{
	if (m_sharedState)
	{
		return m_sharedState->get();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
future<T> future<future<T>>::extract_future() &&
{
	if(m_sharedState)
	{
		embedded_type<future<T>> res = std::move(*m_sharedState).extract_value();

		return std::move(res).extract();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
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
		auto res = std::move(*std::move(*m_sharedState).extract_value()).extract_value();

		return std::move(res);
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
TEMPLATE(typename Return,typename Type)
REQUIRED(IS_CONSTRUCTIBLE(Type,rreference))
future<Return> future<future<T>>::then(const function<Return(Type)>& i_continuation)&&
{
	if(detail::private_async_state_dist_ptr<future<T>> sharedState = m_sharedState)
	{
		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<Return,void>::value)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			const unsigned int currDepth = m_depth;

			future<Return> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}

	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Return,typename Type,typename Context)
REQUIRED(IS_CONSTRUCTIBLE(Type,rreference))
future<Return> future<future<T>>::then_on(const function<Return(Type)>& i_continuation, Context&& i_execContext) &&
{
	if(detail::private_async_state_dist_ptr<future<T>> sharedState = m_sharedState)
	{
		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<Return,void>::value)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}));

		const unsigned int currDepth = m_depth;

		future<Return> res = executor->attach(std::move(i_execContext),currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Return,typename Type,typename Context)
REQUIRED(IS_CONSTRUCTIBLE(Type,rreference))
future<Return> future<future<T>>::async(const function<Return(Type)>& i_continuation, Context&& i_execContext) &&
{
	if(m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		future<Return> res = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<Return,void>::value)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}))->attach(std::move(i_execContext),currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	if(detail::private_async_state_dist_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(acquiredFuture).extract_value();
				}
				else
				{
					return std::move(acquiredFuture).extract_value();
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
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<T> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&
{
	if(detail::private_async_state_dist_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		future<T> res = make_async_executor(make_function([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(acquiredFuture).extract_value();
				}
				else
				{
					return std::move(acquiredFuture).extract_value();
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
		}))->attach(i_execContext,currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}

}
