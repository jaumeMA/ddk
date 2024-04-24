
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
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto shared_future<T>::then(Callable&& i_continuation) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		variant_allocator<system_allocator,allocator_interface_proxy> asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		shared_future<callable_return> res = ddk::async([_sharedState = lend(sharedState),i_continuation]() mutable
		{
			if constexpr(mpl::is_void<callable_return>)
			{
				eval(i_continuation,_sharedState->get_value());
			}
			else
			{
				return eval(i_continuation,_sharedState->get_value());
			}
		}) -> store<executor_promise<callable_return>>(asyncAllocator)
		-> schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable, typename Context)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto shared_future<T>::then_on(Callable&& i_continuation, Context&& i_execContext) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		variant_allocator<system_allocator,allocator_interface_proxy> asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		shared_future<callable_return> res = ddk::async([_sharedState = lend(sharedState),i_continuation,_execContext = std::forward<Context>(i_execContext)]() mutable
		{
			shared_future<callable_return> nestedFuture = ddk::async(i_continuation(_sharedState->get_value())) -> attach(std::forward<Context>(_execContext));

			if constexpr(mpl::is_void<callable_return>)
			{
				nestedFuture.wait();
			}
			else
			{
				return std::move(nestedFuture).extract_value();
			}
		}) -> store<executor_promise<callable_return>>(asyncAllocator)
		-> schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return std::move(res);
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
TEMPLATE(typename Callable,typename Context)
REQUIRED(IS_CALLABLE_BY(Callable,rreference))
auto shared_future<T>::async(Callable&& i_continuation, Context&& i_execContext) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = m_sharedState)
	{
		typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return;

		variant_allocator<system_allocator,allocator_interface_proxy> asyncAllocator = g_system_allocator;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		return ddk::async(make_function([_sharedState = lend(sharedState),i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,_sharedState->get_value());
			}
			else
			{
				return eval(i_continuation,_sharedState->get_value());
			}
		})) -> store<executor_promise<callable_return>>(asyncAllocator)
		-> schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> attach(std::forward<Context>(i_execContext));
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
		variant_allocator<system_allocator,allocator_interface_proxy> asyncAllocator = g_system_allocator;
		executor_context_lent_ptr context = nullptr;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			context = asyncExecutor->get_execution_context();

			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		shared_future<T> res = ddk::async([_sharedState = lend(sharedState),i_onError]() mutable
		{
			try
			{
				if constexpr(mpl::is_void<T>)
				{
					_sharedState->get_value();
				}
				else
				{
					return _sharedState->get_value();
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
		}) -> store<executor_promise<T>>(asyncAllocator)
		-> schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> attach<detail::execution_context_executor>(context,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
shared_future<T> shared_future<T>::on_error(const function<void(const async_error&)>& i_onError, executor_context_lent_ptr i_execContext) const
{
	if(detail::private_async_state_shared_ptr<T> sharedState = this->m_sharedState)
	{
		variant_allocator<system_allocator,allocator_interface_proxy> asyncAllocator = g_system_allocator;

		if (async_base_dist_ptr asyncExecutor = sharedState->get_async_execution())
		{
			if (allocator_const_lent_ptr alloc = asyncExecutor->get_async_allocator())
			{
				asyncAllocator = allocator_interface_proxy{ *alloc };
			}
		}

		shared_future<T> res = ddk::async(make_function([_sharedState = lend(sharedState),i_onError]() mutable
		{
			try
			{
				if constexpr(mpl::is_void<T>)
				{
					_sharedState->get_value();
				}
				else
				{
					return _sharedState->get_value();
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
		})) -> store<executor_promise<T>>(asyncAllocator)
		-> schedule<chained_async_scheduler>(promote_to_ref(sharedState))
		-> attach<detail::execution_context_executor>(i_execContext,m_depth);

		res.m_depth = m_depth + 1;

		return res;
	}

	throw future_exception("Accessing empty future");
}

TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
auto shared_future<void>::then(Callable&& i_continuation) const
{
	return { shared_future<detail::void_t>::then(make_function([i_continuation](const detail::void_t&) { eval(i_continuation); })) };
}
TEMPLATE(typename Callable, typename Context)
REQUIRED(IS_CALLABLE(Callable))
auto shared_future<void>::then_on(Callable&& i_continuation, Context&& i_execContext) const
{
	return shared_future<detail::void_t>::then_on([continuation=std::forward<Callable>(i_continuation)](const detail::void_t&) { eval(i_continuation); },std::forward<Context>(i_execContext));
}
TEMPLATE(typename Callable, typename Context)
REQUIRED(IS_CALLABLE(Callable))
auto shared_future<void>::async(Callable&& i_continuation, Context&& i_execContext) const
{
	return shared_future<detail::void_t>::async([continuation=std::forward<Callable>(i_continuation)](const detail::void_t&) { eval(i_continuation); },std::forward<Context>(i_execContext));
}
auto shared_future<void>::on_error(const function<void(const async_error&)>& i_onError) const
{
	return shared_future<detail::void_t>::on_error(i_onError);
}
auto shared_future<void>::on_error(const function<void(const async_error&)>& i_onError,executor_context_lent_ptr i_execContext) const
{
	return shared_future<detail::void_t>::on_error(i_onError,i_execContext);
}

}