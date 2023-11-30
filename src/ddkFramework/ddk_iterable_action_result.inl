
namespace ddk
{
namespace detail
{

template<typename ActionTag>
iterable_action_recovery<ActionTag,true>::iterable_action_recovery(const result_success_t&)
{
}
template<typename ActionTag>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(recovery_tag,Args...))
iterable_action_recovery<ActionTag,true>::iterable_action_recovery(Args&& ... i_args)
: m_recovery(recovery_tag{ std::forward<Args>(i_args)... })
{
}
template<typename ActionTag>
iterable_action_recovery<ActionTag,true>::operator bool() const
{
	return m_recovery.empty();
}
template<typename ActionTag>
bool iterable_action_recovery<ActionTag,true>::recoverable() const
{
	return m_recovery.empty() == false;
}
template<typename ActionTag>
typename iterable_action_recovery<ActionTag,true>::recovery_tag iterable_action_recovery<ActionTag,true>::recovery() const
{
	return *m_recovery;
}

template<typename ActionTag>
iterable_action_recovery<ActionTag,false>::iterable_action_recovery(const result_success_t&)
: m_success(true)
{
}
template<typename ActionTag>
iterable_action_recovery<ActionTag,false>::operator bool() const
{
	return m_success;
}
template<typename ActionTag>
bool iterable_action_recovery<ActionTag,false>::recoverable() const
{
	return false;
}
template<typename ActionTag>
ActionTag iterable_action_recovery<ActionTag,false>::recovery() const
{
	//you shouldnt reach this point
	throw iterable_operation_forbidden_exception{"You cannot recover from non recoverable actions"};
}

template<typename ActionTag>
iterable_action_result_impl<ActionTag>::iterable_action_result_impl(const result_success_t& i_success)
: base_t(i_success)
{
}
template<typename ActionTag>
template<typename ... Args>
iterable_action_result_impl<ActionTag>::iterable_action_result_impl(Args&& ... i_args)
: base_t(std::forward<Args>(i_args)...)
{
}

}
}