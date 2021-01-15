
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
filtered_iterable_impl<Traits>::filtered_iterable_impl(iterable_impl_shared_ref<iterable_base_traits> i_iterableRef, const function<bool(const_reference)>& i_filter)
: m_iterableRef(i_iterableRef)
, m_filter(i_filter)
{
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](reference i_value) mutable -> action { if(eval(m_filter,i_value)) actionResult = eval(i_try,i_value); if(actionResult.template is_base_of<shift_action>()) actionResult.template get_as<shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
	m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](const_reference i_value) mutable -> action { if(eval(m_filter,i_value)) actionResult = eval(i_try, i_value); if(actionResult.template is_base_of<shift_action>()) actionResult.template get_as<shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
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

                            return go_next_place;
                        };

	lendable<action_state> actionState;
    m_iterableRef->iterate_impl(sizeChecker,go_no_place,lend(actionState));

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

                                stop_iteration();
                            }

							return go_next_place;
                        };

	lendable<action_state> actionState;
	m_iterableRef->iterate_impl(emptyChecker,go_no_place,lend(actionState));

    return res;
}

}
}
