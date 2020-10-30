
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_fiber_utils.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_adaptor.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Iterable,typename Adaptor>
template<typename Action>
action_visitor_base<Iterable,Adaptor>::action_visitor_base(Iterable& i_iterable, Action&& i_initialAction)
: m_adaptor(i_iterable,i_initialAction)
{
}
template<typename Iterable,typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const stop_action& i_action)
{
	iterable_state::forward_result(make_result<action_result>(i_action));

	suspend();

	return m_adaptor.get_value();
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const erase_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		if(optional<reference> res = m_adaptor.remove_item())
		{
			iterable_state::forward_result(make_result<action_result>(i_action));

			return *res;
		}
		else
		{
			iterable_state::forward_result(make_error<action_result>(iter::ActionError::RemovalError,EraseActionError::NonExistingValue));

			return m_adaptor.get_value();
		}
	}
    else
    {
		iterable_state::forward_result(make_error<action_result>(iter::ActionError::RemovalError,EraseActionError::ErasingFromConstantIterable));

        return m_adaptor.get_value();
    }
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const add_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		typedef typename Iterable::value_type value_type;

		if(optional<reference> res = m_adaptor.add_item(i_action.template get<value_type>()))
		{
			iterable_state::forward_result(make_result<action_result>(i_action));

			return *res;
		}
		else
		{
			iterable_state::forward_result(make_error<action_result>(iter::ActionError::AdditionError,AddActionError::NonConvertibleType));

			return m_adaptor.get_value();
		}
    }
    else
    {
		iterable_state::forward_result(make_error<action_result>(iter::ActionError::AdditionError,AddActionError::AddingToConstantIterable));

        return m_adaptor.get_value();
    }
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const go_forward_action& i_action)
{
	if(optional<reference> nextValue = m_adaptor.next_value())
	{
		iterable_state::forward_result(make_result<action_result>(i_action));

		return *nextValue;
	}
	else
	{
		iterable_state::forward_result(make_error<action_result>(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));

		suspend();

		return m_adaptor.get_value();
	}
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,bidirectional_action,Adaptor>::visit(const go_backward_action& i_action)
{
	if(optional<reference> nextValue = m_adaptor.next_value())
	{
		iterable_state::forward_result(make_result<action_result>(i_action));

		return *nextValue;
	}
	else
	{
		iterable_state::forward_result(make_error<action_result>(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));

		suspend();

		return m_adaptor.get_value();
	}
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,random_access_action,Adaptor>::visit(const shift_action& i_action)
{
	if(i_action.shifting() != 0)
	{
		const bool forward = i_action.shifting() > 0;

		if(optional<reference> nextValue = m_adaptor.shift_value(forward ? 1 : -1))
		{
			iterable_state::forward_result(make_result<action_result>(forward ? any_action(go_next_place) : any_action(go_prev_place)));

			return *nextValue;
		}
		else
		{
			iterable_state::forward_result(make_error<action_result>(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));

			suspend();

			return m_adaptor.get_value();
		}
	}
	else
	{
		iterable_state::forward_result(make_result<action_result>(i_action));

		return m_adaptor.get_value();
	}
}

}

template<typename Iterable, typename Action, typename Reference>
Reference visit_iterator(Iterable& i_iterable, const function<Action(Reference)>& i_sink, const iter::shift_action& i_initialAction)
{
	detail::action_visitor<Iterable,Action,typename detail::iterable_adaptor<Iterable,Action>::type> actionVisitor(i_iterable,i_initialAction);
	Action currAction(go_to_place(0));

	while(true)
	{
        currAction = eval(i_sink,currAction.visit(actionVisitor));
	}

	return ddk::crash_on_return<Reference>::value();
}

}
}
