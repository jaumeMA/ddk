
namespace ddk
{
namespace detail
{

template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
context_private_async_state<T>::context_private_async_state(Args&& ... i_args)
: private_async_state<T>(std::forward<Args>(i_args)...)
{
	this->attach(this->distributed_from_this());
}
template<typename T>
context_private_async_state<T>::~context_private_async_state()
{
	this->detach();
}
template<typename T>
executor_context_lent_ptr context_private_async_state<T>::get_execution_context()
{
	return this->lent_from_this();
}
template<typename T>
executor_context_const_lent_ptr context_private_async_state<T>::get_execution_context() const
{
	return this->lent_from_this();
}
template<typename T>
allocator_const_lent_ptr context_private_async_state<T>::get_async_allocator() const
{
	return nullptr;
}
template<typename T>
typename context_private_async_state<T>::cancel_result context_private_async_state<T>::cancel()
{
	if (this->ready())
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
	else
	{
		this->set_exception(async_exception{ "Promise has been cancelled by user",AsyncExceptionCode::Cancelled });

		return success;
	}
}
template<typename T>
void context_private_async_state<T>::hold(private_async_state_base_shared_ref i_sharedState)
{
}

}
}