
namespace ddk
{

template<typename T>
future<future<T>>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
, m_depth(other.m_depth)
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
	m_depth = other.m_depth;

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
constexpr auto future<future<T>>::then(Callable&& i_continuation) &&
{
	if (detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		return contraction(std::move(*this)._async([sharedState,_continuation = std::forward<Callable>(i_continuation)](future<T> i_future) mutable
		{
			return std::move(i_future).chain(promote_to_ref(sharedState))
			.then(std::forward<Callable>(_continuation));
		}));

		//if no context available, future should be ready so this is a regular then
		return std::move(*this).extract_value().then(std::forward<Callable>(i_continuation));
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
constexpr future<future<T>> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError)&&
{
	if(detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		typedef variant_allocator<system_allocator,allocator_interface_proxy> async_allocator;

		async_allocator asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		future<future<T>> res = ddk::async([_sharedState = lend(sharedState),i_onError]() mutable -> T
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(*std::move(*_sharedState).extract_value());
				}
				else
				{
					return std::move(*std::move(*_sharedState).extract_value());
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					ddk::eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}) -> template schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> template attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
constexpr future<future<T>> future<future<T>>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext)&&
{
	if(detail::private_async_state_shared_ptr<future<T>> sharedState = m_sharedState)
	{
		typedef variant_allocator<system_allocator,allocator_interface_proxy> async_allocator;

		async_allocator asyncAllocator = g_system_allocator;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		future<T> res = ddk::async([_sharedState = lend(sharedState),i_onError]() mutable -> T
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					std::move(*std::move(*_sharedState).extract_value());
				}
				else
				{
					return std::move(*std::move(*_sharedState).extract_value());
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					ddk::eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}) -> template schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> template attach<detail::execution_context_executor>(i_execContext,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<future<T>>::_then(Callable && i_continuation) &&
{
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return_type;

	if (detail::private_async_state_shared_ptr<future<T>> sharedState = std::move(m_sharedState))
	{
		typedef variant_allocator<system_allocator,allocator_interface_proxy> async_allocator;

		async_allocator asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		future<callable_return_type> res = ddk::async([_sharedState = lend(sharedState),continuation=std::forward<Callable>(i_continuation)]() mutable -> callable_return_type
		{
			if constexpr (mpl::is_void<callable_return_type>)
			{
				ddk::eval(std::forward<Callable>(continuation),std::move(*_sharedState).extract_value());
			}
			else
			{
				return ddk::eval(std::forward<Callable>(continuation),std::move(*_sharedState).extract_value());
			}
		}) -> template schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> template attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
constexpr auto future<future<T>>::_async(Callable&& i_continuation) &&
{
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return_type;

	if (detail::private_async_state_shared_ptr<future<T>> sharedState = std::move(m_sharedState))
	{
		typedef variant_allocator<system_allocator,allocator_interface_proxy> async_allocator;

		async_allocator asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		future<callable_return_type> res = ddk::async([_sharedState = lend(sharedState),continuation=std::forward<Callable>(i_continuation)]() mutable -> callable_return_type
		{
			if constexpr (mpl::is_void<callable_return_type>)
			{
				ddk::eval(std::forward<Callable>(continuation),std::move(*_sharedState).extract_value());
			}
			else
			{
				return ddk::eval(std::forward<Callable>(continuation),std::move(*_sharedState).extract_value());
			}
		}) -> template schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> template attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}

}
