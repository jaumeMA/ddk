
#include "ddk_iterable_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_filter<Function>::iterable_filter(const Function& i_filter)
: m_filter(i_filter)
{
}
template<typename Function>
Function iterable_filter<Function>::get_filter() const
{
	return m_filter;
}

template<typename Traits>
filtered_iterable_impl<Traits>::filtered_iterable_impl(iterable_impl_shared_ref<iterable_base_traits> i_iterableRef, const function<bool(reference)>& i_filter)
: m_iterableRef(i_iterableRef)
, m_filter(i_filter)
{
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr)
{
	m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](reference i_value) mutable -> action { if(eval(m_filter,i_value)) actionResult = eval(i_try,i_value); if(actionResult.is_base_of<iter::shift_action>()) actionResult.get_as<iter::shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr) const
{
	m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](const_reference i_value) mutable -> action { if(eval(m_filter,i_value)) actionResult = eval(i_try, i_value); if(actionResult.is_base_of<iter::shift_action>()) actionResult.get_as<iter::shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
}
template<typename Traits>
size_t filtered_iterable_impl<Traits>::size() const
{
    size_t res = 0;

    auto sizeChecker = [&res,this](const_reference i_value) mutable -> action
                        {
                            if(eval(m_filter,i_value))
                            {
                                ++res;
                            }

                            return iter::go_next_place;
                        };

	lendable<iter::action_state> actionState;
    m_iterableRef->iterate_impl(sizeChecker,iter::go_no_place,lend(actionState));

    return res;
}
template<typename Traits>
bool filtered_iterable_impl<Traits>::empty() const
{
    bool res = true;

    auto emptyChecker = [&res,this](const_reference i_value) mutable -> action
                        {
                            if(eval(m_filter,i_value))
                            {
                                res = false;

                                return iter::stop_iteration;
                            }
                            else
                            {
                                return iter::go_next_place;
                            }
                        };

	lendable<iter::action_state> actionState;
	m_iterableRef->iterate_impl(emptyChecker,iter::go_no_place,lend(actionState));

    return res;
}

}
}
