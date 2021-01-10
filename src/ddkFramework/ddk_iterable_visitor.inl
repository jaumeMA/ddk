
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

template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
template<typename Action, typename FFunction>
action_visitor_base<Iterable,FinalAction,Function,Adaptor>::action_visitor_base(Iterable& i_iterable, Action&& i_initialAction, FFunction&& i_sink, action_state_lent_ptr i_actionStatePtr)
: m_adaptor(i_iterable,i_initialAction)
, m_actionStatePtr(i_actionStatePtr)
, m_currAction(go_no_place)
, m_sink(std::forward<FFunction>(i_sink))
{
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
bool action_visitor_base<Iterable,FinalAction,Function,Adaptor>::valid() const noexcept
{
	return m_adaptor.valid();
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
template<typename Visitor>
void action_visitor_base<Iterable,FinalAction,Function,Adaptor>::loop()
{
	Visitor& thisVisitor = static_cast<Visitor&>(*this);

	while(m_currAction.visit(thisVisitor));
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
template<typename T>
void action_visitor_base<Iterable,FinalAction,Function,Adaptor>::operator()(T&& i_value) const
{
	m_currAction = eval(m_sink,std::forward<T>(i_value));
}

template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::visit(const go_forward_action& i_action)
{
	if(m_adaptor.forward_next_value_in(*this))
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(1,1));
		}

		return true;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::visit(const stop_action& i_action)
{
	if(m_actionStatePtr)
	{
		m_actionStatePtr->forward_result(i_action);
	}

	return false;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::visit(const erase_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		if(m_adaptor.forward_erase_value_in(*this))
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(i_action);
			}
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::RemovalError,EraseActionError::NonExistingValue));
			}
		}
	}
    else
    {
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::RemovalError,EraseActionError::ErasingFromConstantIterable));
		}
    }

	return true;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::visit(const add_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		typedef typename Iterable::value_type value_type;

		if(m_adaptor.forward_add_value_in(i_action.template get<value_type>(),*this))
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(i_action);
			}
		}
		else
		{
			if(m_actionStatePtr)
			{
				m_actionStatePtr->forward_result(action_error(iter::ActionError::AdditionError,AddActionError::NonConvertibleType));
			}
		}
    }
    else
    {
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::AdditionError,AddActionError::AddingToConstantIterable));
		}
    }

	return true;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>::visit(const go_backward_action& i_action)
{
	if(m_adaptor.forward_prev_value_in(*this))
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return true;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>::visit(const go_backward_action& i_action)
{
	if(m_adaptor.forward_prev_value_in(*this))
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return true;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_random_access_action,Adaptor>::visit(const shift_action& i_action)
{
	const int currShift = i_action.shifting();

	if(m_adaptor.forward_shift_value_in(currShift,*this))
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(currShift,currShift));
		}

		return true;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError{ShiftActionError::ShiftOutOfBounds}));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,random_access_action,Adaptor>::visit(const shift_action& i_action)
{
	const int currShift = i_action.shifting();

	if(m_adaptor.forward_shift_value_in(currShift,*this))
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(go_to_place(currShift,currShift));
		}

		return true;
	}
	else
	{
		if(m_actionStatePtr)
		{
			m_actionStatePtr->forward_result(action_error(iter::ActionError::ShiftError,ShiftActionError::ShiftOutOfBounds));
		}

		return false;
	}
}

}

template<typename Iterable, typename Function, typename Action>
void visit_iterator(Iterable& i_iterable, Function&& i_sink, const Action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	typedef detail::action_visitor<Iterable,Action,Function,Action,typename detail::iterable_adaptor<Iterable,Action>::type> action_visitor_t;
	action_visitor_t actionVisitor(i_iterable,i_initialAction.get_as<iter::shift_action>(),std::forward<Function>(i_sink),i_actionStatePtr);

	if(actionVisitor.valid())
	{
		actionVisitor.loop<action_visitor_t>();
	}

	suspend();
}

}
}
