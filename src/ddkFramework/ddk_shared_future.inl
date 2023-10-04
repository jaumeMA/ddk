
namespace ddk
{

template<typename T>
shared_future<T>::shared_future(const shared_future& i_future)
: future<T>(i_future.m_sharedState,i_future.m_depth)
{
}
template<typename T>
shared_future<T>::shared_future(shared_future&& i_future)
: future<T>(std::move(i_future.m_sharedState),i_future.m_depth)
{
}
template<typename T>
shared_future<T>::shared_future(future<T>&& i_future)
: future<T>(std::move(i_future))
{
}
template<typename T>
shared_future<T>& shared_future<T>::operator=(const shared_future& other)
{
	this->m_sharedState = other.m_sharedState;
	this->m_depth = other.m_depth;

	return *this;
}
template<typename T>
shared_future<T>& shared_future<T>::operator=(shared_future&& other)
{
	this->m_sharedState = std::move(other.m_sharedState);
	this->m_depth = other.m_depth;

	return *this;
}
template<typename T>
template<typename TT>
shared_future<TT> shared_future<T>::then(const function<TT(const_reference)>& i_continuation) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = ddk::async(make_function([acquiredFuture = *this,i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.get_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.get_value());
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			future<TT> res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return static_cast<future<T>>(std::move(executor));
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT,typename TTT>
shared_future<TT> shared_future<T>::then_on(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = ddk::async(make_function([acquiredFuture = *this,i_continuation,acquiredExecContext = std::forward<TTT>(i_execContext)]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				shared_future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.get_value()))->attach(std::forward<TTT>(acquiredExecContext),acquiredFuture.m_depth);

				nestedFuture.wait();
			}
			else
			{
				shared_future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.get_value()))->attach(std::forward<TTT>(acquiredExecContext),acquiredFuture.m_depth);

				return std::move(nestedFuture).extract_value();
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			future<TT> res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return static_cast<future<T>>(std::move(executor));
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT,typename TTT>
shared_future<TT> shared_future<T>::async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const
{
	if(this->m_sharedState)
	{
		return ddk::async(make_function([acquiredFuture = *this,i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.get_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.get_value());
			}
		}))->attach(std::forward<TTT>(i_execContext));
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
template<typename TT>
shared_future<TT> shared_future<T>::async(const function<TT(const_reference)>& i_continuation,executor_context_lent_ptr i_execContext) const
{
	if(this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		future<TT> res = ddk::async(make_function([acquiredFuture = *this,i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
			else
			{
				return eval(i_continuation, std::move(acquiredFuture).extract_value());
			}
		}))->attach(i_execContext,currDepth);

		res.m_depth = currDepth + 1;

		return std::move(res);
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
shared_future<T> shared_future<T>::on_error(const function<void(const async_error&)>& i_onError) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = ddk::async(make_function([acquiredFuture = *this,i_onError]() mutable
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

		if(async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			future<T> res = std::move(executor) -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return static_cast<future<T>>(std::move(executor));
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
shared_future<T> shared_future<T>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		future<T> res = ddk::async(make_function([acquiredFuture = *this,i_onError]() mutable
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