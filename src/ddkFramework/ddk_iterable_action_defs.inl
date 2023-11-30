
#include "ddk_fiber_utils.h"
#include "ddk_iterable_adaptor_concepts.h"

namespace ddk
{

template<typename Adaptor,typename Sink>
no_action no_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	return {};
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto remove_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (i_adaptor.perform_action(remove_action_tag{}))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

template<typename T>
add_action<T>::add_action(T i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
add_action<T>::add_action(const stop_action&)
: action_base(false)
{
}
template<typename T>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto add_action<T>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (i_adaptor.perform_action(add_action_tag<T>{std::move(m_value)}))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
auto forward_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (i_adaptor.perform_action(forward_action_tag{}))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
auto backward_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (i_adaptor.perform_action(backward_action_tag{}))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
auto go_to_begin_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_sink;
	typedef typename mpl::which_type<mpl::is_same_type<return_sink,go_to_begin_action>::value,forward_action,return_sink>::type return_type;

	if (i_adaptor.perform_action(begin_action_tag{}))
	{
		return return_type{ i_adaptor.forward_value(std::forward<Sink>(i_sink)) };
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
auto go_to_end_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_sink;
	typedef typename mpl::which_type<mpl::is_same_type<return_sink,go_to_end_action>::value,backward_action,return_sink>::type return_type;

	if (i_adaptor.perform_action(last_action_tag{}))
	{
		return return_type{ i_adaptor.forward_value(std::forward<Sink>(i_sink)) };
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto bidirectional_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	if (m_forward)
	{
		static const forward_action k_forwardAction;

		return k_forwardAction.apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink));
	}
	else
	{
		static const backward_action k_backwardAction;

		return k_backwardAction.apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink));
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto displacement_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (i_adaptor.perform_action(displace_action_tag{ shift() }))
	{
		return return_type{ i_adaptor.forward_value(std::forward<Sink>(i_sink)) };
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

template<typename Action>
TEMPLATE(typename AAction)
REQUIRED(IS_CONSTRUCTIBLE(Action,AAction))
step_by_step_action<Action>::step_by_step_action(AAction&& i_action)
: m_action(std::forward<AAction>(i_action))
{
	if constexpr (IS_BASE_OF_COND(displacement_action,AAction))
	{
		m_steps = std::abs(i_action.shift());

		i_action.set_shift((i_action.shift() > 0) ? 1 : -1);
	}
}
template<typename Action>
TEMPLATE(typename AAction)
REQUIRED(IS_CONSTRUCTIBLE(Action,AAction))
step_by_step_action<Action>::step_by_step_action(const step_by_step_action<AAction>& other)
: m_action(other.m_action)
, m_steps(other.m_steps)
{
}
template<typename Action>
step_by_step_action<Action>::step_by_step_action(const stop_action& i_action)
: action_base(false)
, m_action(i_action)
{
}
template<typename Action>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto step_by_step_action<Action>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	return m_action.apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink));
}
template<typename Action>
step_by_step_action<Action>& step_by_step_action<Action>::operator--()
{
	m_steps--;

	return *this;
}
template<typename Action>
bool step_by_step_action<Action>::ready() const
{
	return m_steps == 0;
}

template<typename Action,typename AAction>
action_pair<Action,AAction>::action_pair(const Action& i_lhs, const AAction& i_rhs)
: Action(i_lhs)
,m_pendingAction(i_rhs)
{
}
template<typename Action,typename AAction>
action_pair<Action,AAction>::action_pair(const stop_action& i_action)
: Action(i_action)
, m_pendingAction(i_action)
{
}
template<typename Action,typename AAction>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto action_pair<Action,AAction>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	if (const auto newAction = Action::apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink)))
	{
		return m_pending_action;
	}
	else
	{
		return AAction{ stop_iteration };
	}
}

}