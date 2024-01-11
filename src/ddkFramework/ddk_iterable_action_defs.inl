
#include "ddk_iterable_action_result.h"

namespace ddk
{

template<typename Adaptor>
auto no_action::apply(Adaptor&& i_adaptor) const
{
	return make_result<iterable_action_result<no_action>>();
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto remove_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(remove_action_tag{}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not remove action");
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
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto add_action<T>::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(add_action_tag<T>{std::move(m_value)}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not add action");
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
auto forward_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(forward_action_tag{}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not forward action");
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
auto backward_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(backward_action_tag{}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not backward action");
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
auto go_to_begin_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(begin_action_tag{}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not begin action");
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
auto go_to_end_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(last_action_tag{}))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not end action");
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto bidirectional_action::apply(Adaptor&& i_adaptor) const
{
	if (m_forward)
	{
		static const forward_action k_forwardAction;

		return k_forwardAction.apply(std::forward<Adaptor>(i_adaptor));
	}
	else
	{
		static const backward_action k_backwardAction;

		return k_backwardAction.apply(std::forward<Adaptor>(i_adaptor));
	}
}

TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto displacement_action::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = i_adaptor.perform_action(displace_action_tag{ shift() }))
	{
		return make_result<iterable_action_result<no_action>>();
	}
	else
	{
		return make_error<iterable_action_result<no_action>>(IterableError::InternalError,"Could not displace action");
	}
}

template<typename Sink>
sink_action<Sink>::sink_action(const Sink& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
sink_action<Sink>::sink_action(Sink&& i_sink)
: m_sink(i_sink)
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
sink_action<Sink>::sink_action(const sink_action<SSink>& i_action)
: m_sink(i_action.m_sink)
{
}
template<typename Sink>
TEMPLATE(typename SSink)
REQUIRED(IS_CONSTRUCTIBLE(Sink,SSink))
sink_action<Sink>::sink_action(sink_action<SSink>&& i_action)
: m_sink(std::move(i_action.m_sink))
{
}
template<typename Sink>
sink_action<Sink>::sink_action(const stop_action&)
: action_base(false)
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
	typedef decltype(std::declval<Sink>()(std::declval<reference>())) return_action;
	typedef iterable_action_result<and_action<return_action,sink_action<Sink>>> return_result;

	return_action returnAction(stop_iteration);
	if (auto actionRes = i_adaptor.perform_action(sink_action_tag{ [&](auto&& i_value) mutable { returnAction = ddk::eval(m_sink,std::forward<decltype(i_value)>(i_value)); }}))
	{
		return make_result<return_result>(returnAction && sink_action{m_sink});
	}
	else
	{
		return make_error<return_result>(IterableError::InternalError,"Could not sink action");
	}
}

}