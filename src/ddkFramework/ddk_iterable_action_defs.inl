
#include "ddk_iterable_action_result.h"

namespace ddk
{

constexpr action_base::action_base(bool i_valid)
: m_valid(i_valid)
{
}
constexpr action_base::operator bool() const
{
	return m_valid;
}

constexpr no_action::no_action(bool i_valid)
: action_base(i_valid)
{
}
template<typename Adaptor>
constexpr no_action no_action::apply(Adaptor&& i_adaptor) const
{
	return {};
}

constexpr remove_action::remove_action(bool i_valid)
: action_base(i_valid)
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr no_action remove_action::apply(Adaptor&& i_adaptor) const
{
	i_adaptor.perform_action(remove_action_tag{}).dismiss();

	return {};
}

template<typename T>
constexpr add_action<T>::add_action(T i_value)
: m_value(std::move(i_value))
{
}
template<typename T>
constexpr add_action<T>::add_action(T i_value, bool i_valid)
: action_base(i_valid)
, m_value(std::move(i_value))
{
}
template<typename T>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr no_action add_action<T>::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(add_action_tag<T>{std::move(m_value)})) };
}

constexpr forward_action::forward_action(bool i_valid)
: action_base(i_valid)
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
constexpr forward_action forward_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(forward_action_tag{})) };
}

constexpr backward_action::backward_action(bool i_valid)
: action_base(i_valid)
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
constexpr backward_action backward_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(backward_action_tag{})) };
}

constexpr go_to_begin_action::go_to_begin_action(bool i_valid)
: forward_action(i_valid)
{
}
constexpr go_to_begin_action::go_to_begin_action(const forward_action& i_action)
: forward_action(i_action)
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
constexpr forward_action go_to_begin_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(begin_action_tag{})) };
}

constexpr go_to_end_action::go_to_end_action(bool i_valid)
: backward_action(i_valid)
{
}
constexpr go_to_end_action::go_to_end_action(const backward_action& i_action)
: backward_action(i_action)
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
constexpr backward_action go_to_end_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(last_action_tag{})) };
}

constexpr bidirectional_action::bidirectional_action(bool i_forward)
: m_forward(i_forward)
{
}
constexpr bidirectional_action::bidirectional_action(bool i_forward,bool i_valid)
: action_base(i_valid)
, m_forward(i_forward)
{
}
constexpr bidirectional_action::bidirectional_action(const displacement_action& i_action,bool i_valid)
: action_base(i_valid)
, m_forward((i_action.shift() > 0))
{
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr bidirectional_action bidirectional_action::apply(Adaptor&& i_adaptor) const
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

constexpr displacement_action::displacement_action(difference_type i_targetShift)
: m_shift(i_targetShift)
{
}
constexpr displacement_action::displacement_action(difference_type i_targetShift,bool i_valid)
: action_base(i_valid)
, m_shift(i_targetShift)
{
}
constexpr displacement_action::difference_type displacement_action::shift() const
{
	return m_shift;
}
constexpr void displacement_action::set_shift(difference_type i_shift)
{
	m_shift = i_shift;
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr no_action displacement_action::apply(Adaptor&& i_adaptor) const
{
	return { static_cast<bool>(i_adaptor.perform_action(displace_action_tag{ shift() })) };
}

template<typename Sink>
constexpr sink_action<Sink>::sink_action(const Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
constexpr sink_action<Sink>::sink_action(Sink&& i_sink)
: m_sink(std::move(i_sink))
{
}
template<typename Sink>
constexpr sink_action<Sink>::sink_action(const Sink& i_sink, bool i_valid)
: action_base(i_valid)
, m_sink(i_sink)
{
}
template<typename Sink>
constexpr sink_action<Sink>::sink_action(Sink&& i_sink, bool i_valid)
: action_base(i_valid)
, m_sink(std::move(i_sink))
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
constexpr sink_action<Sink>::sink_action(const sink_action<SSink>& i_action)
: action_base(i_action)
, m_sink(i_action.m_sink)
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
constexpr sink_action<Sink>::sink_action(sink_action<SSink>&& i_action)
: action_base(std::move(i_action))
, m_sink(std::move(i_action.m_sink))
{
}
template<typename Sink>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr auto sink_action<Sink>::apply(Adaptor&& i_adaptor) const
{
	if (auto actionRes = i_adaptor.perform_action(sink_action_tag{ [this](auto&& i_value) mutable { ddk::eval(m_sink,std::forward<decltype(i_value)>(i_value)); } }))
	{
		return sink_action{ m_sink };
	}
	else
	{
		return sink_action{ m_sink,false };
	}
}

template<typename Action,typename Sink>
constexpr action_sink<Action,Sink>::action_sink(const Action& i_action,const Sink& i_sink)
: m_action(i_action)
, m_sink(i_sink)
{
}
template<typename Action,typename Sink>
constexpr action_sink<Action,Sink>::action_sink(Action&& i_action,Sink&& i_sink)
: m_action(std::move(i_action))
, m_sink(std::move(i_sink))
{
}
template<typename Action,typename Sink>
constexpr action_sink<Action,Sink>::action_sink(const Action& i_action,const Sink& i_sink,bool i_valid)
: action_base(i_valid)
, m_action(i_action)
, m_sink(i_sink)
{
}
template<typename Action,typename Sink>
constexpr action_sink<Action,Sink>::action_sink(Action&& i_action,Sink&& i_sink,bool i_valid)
: action_base(i_valid)
, m_action(std::move(i_action))
, m_sink(std::move(i_sink))
{
}
template<typename Action,typename Sink>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr auto action_sink<Action,Sink>::apply(Adaptor&& i_adaptor) const
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

constexpr size_action::size_action(bool i_valid)
: action_base(i_valid)
{
}
constexpr size_t size_action::operator()() const
{
	return m_size;
}
constexpr size_action::operator bool() const
{
	return m_size != nsize;
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr no_action size_action::apply(Adaptor&& i_adaptor) const
{
	if (auto actionRes = i_adaptor.perform_action(size_action_tag{}))
	{
		m_size = actionRes.get();

		return {};
	}
	else
	{
		return { false };
	}
}

}