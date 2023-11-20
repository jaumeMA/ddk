
#include "ddk_fiber_utils.h"
#include "ddk_iterable_adaptor_concepts.h"

namespace ddk
{

template<typename Adaptor,typename Sink>
no_action no_action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	return {};
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
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
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

template<typename Traits>
TEMPLATE(typename Action)
REQUIRED(ACTION_TAGS_SUPPORTED(Traits,typename Action::tags_t))
supported_action<Traits>::supported_action(const Action& i_action)
: m_action(make_function([i_action](iterable_adaptor_wrapper<Traits> i_adaptorWrapper)
{
	return static_cast<bool>(i_action.apply(i_adaptorWrapper,[](auto&&){}));
}))
{
}
template<typename Traits>
supported_action<Traits>::supported_action(const stop_action&)
: action_base(false)
{
}
template<typename Traits>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto supported_action<Traits>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	if (ddk::eval(m_action,i_adaptor))
	{
		return i_adaptor.forward_value(std::forward<Sink>(i_sink));
	}
	else
	{
		return return_type{ stop_iteration };
	}
}

template<typename Action,typename ... Actions>
action_list<Action,Actions...>::action_list(const Action& i_action, const Actions& ... i_actions)
: Action(i_action)
, m_pendingActions({ i_actions... })
{

}
template<typename Action,typename ... Actions>
action_list<Action,Actions...>::action_list(const stop_action& i_action)
: Action(i_action)
, m_pendingActions({ Actions{i_action}... })
{
}
template<typename Action,typename ... Actions>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto action_list<Action,Actions...>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	if constexpr (s_num_pending_actions > 0)
	{
		if (const auto newAction = Action::apply(std::forward<Adaptor>(i_adaptor),[](auto&&) -> no_action { return {}; }))
		{
			typedef typename mpl::make_sequence<0,s_num_pending_actions>::type seq_type;

			return forward_next_action(seq_type{});
		}
		else
		{
			return action_list<Actions...>{ stop_iteration };
		}
	}
	else
	{
		return Action::apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink));
	}
}
template<typename Action,typename ... Actions>
template<size_t ... Indexs>
auto action_list<Action,Actions...>::forward_next_action(const mpl::sequence<Indexs...>&) const
{
	return action_list{ m_pending_actions.template get<Indexs>() ... };
}

}