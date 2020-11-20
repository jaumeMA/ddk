
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
action_visitor_base<Iterable,Adaptor>::action_visitor_base(Iterable& i_iterable, Action&& i_initialAction, action_state_lent_ptr i_actionStatePtr)
: m_adaptor(i_iterable,i_initialAction)
, m_actionStatePtr(i_actionStatePtr)
{
}
template<typename Iterable,typename Adaptor>
bool action_visitor_base<Iterable,Adaptor>::valid() const noexcept
{
	return m_adaptor.valid();
}
template<typename Iterable,typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,const_input_action,Adaptor>::visit(const go_forward_action& i_action)
{
	if(auto nextValue = m_adaptor.next_value())
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(1,1));
		}

		return *nextValue;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		suspend();

		return m_adaptor.get_value();
	}
}
template<typename Iterable,typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const stop_action& i_action)
{
	if(m_actionStatePtr)
	{
		m_actionStatePtr->forward_result(i_action);
	}

	suspend();

	return m_adaptor.get_value();
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const erase_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		if(auto res = m_adaptor.remove_item())
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(i_action);
			}

			return *res;
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::RemovalError,EraseActionError::NonExistingValue));
			}

			return m_adaptor.get_value();
		}
	}
    else
    {
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::RemovalError,EraseActionError::ErasingFromConstantIterable));
		}

		return m_adaptor.get_value();
    }
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,input_action,Adaptor>::visit(const add_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		typedef typename Iterable::value_type value_type;

		if(auto res = m_adaptor.add_item(i_action.template get<value_type>()))
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(i_action);
			}

			return *res;
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::AdditionError,AddActionError::NonConvertibleType));
			}

			return m_adaptor.get_value();
		}
    }
    else
    {
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::AdditionError,AddActionError::AddingToConstantIterable));
		}

        return m_adaptor.get_value();
    }
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,const_bidirectional_action,Adaptor>::visit(const go_backward_action& i_action)
{
	if(auto nextValue = m_adaptor.prev_value())
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return *nextValue;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		suspend();

		return m_adaptor.get_value();
	}
}
template<typename Iterable,typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,bidirectional_action,Adaptor>::visit(const go_backward_action& i_action)
{
	if(auto nextValue = m_adaptor.prev_value())
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return *nextValue;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		suspend();

		return m_adaptor.get_value();
	}
}
template<typename Iterable,typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,const_random_access_action,Adaptor>::visit(const shift_action& i_action)
{
	if(const int shiftingValue = i_action.shifting())
	{
		if(auto nextValue = m_adaptor.shift_value(shiftingValue))
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(go_to_place(shiftingValue,shiftingValue));
			}

			return *nextValue;
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError{ShiftActionError::ShiftOutOfBounds}));
			}

			suspend();

			return m_adaptor.get_value();
		}
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(i_action);
		}

		return m_adaptor.get_value();
	}
}
template<typename Iterable, typename Adaptor>
typename Adaptor::reference action_visitor<Iterable,random_access_action,Adaptor>::visit(const shift_action& i_action)
{
	if(const int shiftingValue = i_action.shifting())
	{
		if(auto nextValue = m_adaptor.shift_value(shiftingValue))
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(go_to_place(shiftingValue,shiftingValue));
			}

			return *nextValue;
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
			}

			suspend();

			return m_adaptor.get_value();
		}
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(i_action);
		}

		return m_adaptor.get_value();
	}
}

}

template<typename Iterable, typename Function>
typename Iterable::reference visit_iterator(Iterable& i_iterable, const Function& i_sink, const iter::shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	typedef typename Function::return_type action_type;
	detail::action_visitor<Iterable,action_type,typename detail::iterable_adaptor<Iterable,action_type>::type> actionVisitor(i_iterable,i_initialAction,i_actionStatePtr);

	if(actionVisitor.valid())
	{
		action_type currAction(go_no_place);

		while(true)
		{
			currAction = i_sink.inline_eval(currAction.visit(actionVisitor));
		}
	}
	else
	{
		suspend();
	}

	return ddk::crash_on_return<typename Iterable::reference>::value();
}

}
}
