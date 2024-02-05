
#include "ddk_iterable_action_result.h"

namespace ddk
{

template<typename Adaptor>
no_action no_action::apply(Adaptor&& i_adaptor) const
{
	return {};
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
no_action remove_action::apply(Adaptor&& i_adaptor) const
{
	i_adaptor.perform_action(remove_action_tag{}).dismiss();

	return {};
}

template<typename T>
add_action<T>::add_action(T i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
add_action<T>::add_action(T i_value, bool i_valid)
: action_base(i_valid)
, m_value(std::move(i_value))
{
}
template<typename T>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
no_action add_action<T>::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(add_action_tag<T>{std::move(m_value)})) };
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
forward_action forward_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(forward_action_tag{})) };
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
backward_action backward_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(backward_action_tag{})) };
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
forward_action go_to_begin_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(begin_action_tag{})) };
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
backward_action go_to_end_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(last_action_tag{})) };
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
bidirectional_action bidirectional_action::apply(Adaptor&& i_adaptor) const
{
	if (m_forward)
	{
		return { m_forward,static_cast<bool>(i_adaptor.perform_action(forward_action_tag{})) };
	}
	else
	{
		return { m_forward,static_cast<bool>(i_adaptor.perform_action(backward_action_tag{})) };
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
no_action displacement_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(displace_action_tag{ shift() })) };
}

template<typename Sink>
sink_action<Sink>::sink_action(const Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
sink_action<Sink>::sink_action(Sink&& i_sink)
: m_sink(std::move(i_sink))
{
}
template<typename Sink>
sink_action<Sink>::sink_action(const Sink& i_sink, bool i_valid)
: action_base(i_valid)
, m_sink(i_sink)
{
}
template<typename Sink>
sink_action<Sink>::sink_action(Sink&& i_sink, bool i_valid)
: action_base(i_valid)
, m_sink(std::move(i_sink))
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
sink_action<Sink>::sink_action(const sink_action<SSink>& i_action)
: action_base(i_action)
, m_sink(i_action.m_sink)
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
sink_action<Sink>::sink_action(sink_action<SSink>&& i_action)
: action_base(std::move(i_action))
, m_sink(std::move(i_action.m_sink))
{
}
template<typename Sink>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto sink_action<Sink>::apply(Adaptor&& i_adaptor) const
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;
	typedef typename adaptor_t::traits traits;
	typedef typename traits::reference reference;
	typedef decltype(std::declval<Sink>()(std::declval<reference>())) sink_return_action;
	typedef and_action<sink_return_action,sink_action<Sink>> return_action;

	sink_return_action returnAction;
	if (auto actionRes = i_adaptor.perform_action(sink_action_tag{ [&](auto&& i_value) mutable { returnAction = ddk::eval(m_sink,std::forward<decltype(i_value)>(i_value)); } }))
	{
		return return_action(returnAction,*this);
	}
	else
	{
		return return_action(returnAction,*this,false);
	}
}

template<typename Action,typename Sink>
action_sink<Action,Sink>::action_sink(const Action& i_action,const Sink& i_sink)
: m_action(i_action)
, m_sink(i_sink)
{
}
template<typename Action,typename Sink>
action_sink<Action,Sink>::action_sink(Action&& i_action,Sink&& i_sink)
: m_action(std::move(i_action))
, m_sink(std::move(i_sink))
{
}
template<typename Action,typename Sink>
action_sink<Action,Sink>::action_sink(const Action& i_action,const Sink& i_sink,bool i_valid)
: action_base(i_valid)
, m_action(i_action)
, m_sink(i_sink)
{
}
template<typename Action,typename Sink>
action_sink<Action,Sink>::action_sink(Action&& i_action,Sink&& i_sink,bool i_valid)
: action_base(i_valid)
, m_action(std::move(i_action))
, m_sink(std::move(i_sink))
{
}
template<typename Action,typename Sink>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto action_sink<Action,Sink>::apply(Adaptor&& i_adaptor) const
{
	if (auto nextAction = m_action.apply(std::forward<Adaptor>(i_adaptor)))
	{
		i_adaptor.perform_action(sink_action_tag{ [this](auto&& i_value) mutable { ddk::eval(m_sink,std::forward<decltype(i_value)>(i_value)); } }).dismiss();

		return action_sink<decltype(nextAction),Sink>{ nextAction,m_sink };
	}
	else
	{
		return action_sink<decltype(nextAction),Sink>{ nextAction,m_sink,false };
	}
}

}