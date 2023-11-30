
namespace ddk
{
namespace detail
{

template<bool Forward>
TEMPLATE(typename Action)
REQUIRED(ANY_ACTION_TAGS_SUPPORTED(Action,forward_action_tag,backward_action_tag,displace_action_tag))
auto reversable_order_resolver<Forward>::operator()(const Action& i_action) const
{
	if constexpr (Forward)
	{
		return i_action;
	}
	else
	{
		return inverse(i_action);
	}
}

TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto transpose_multi_dimensional_order_resolver::action::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	typedef typename mpl::remove_qualifiers<Adaptor>::dimension_t adaptor_dimension;
	typedef typename adaptor_dimension::template drop<0> adaptor_sub_dimension;

	typedef decltype(std::declval<Adaptor>().forward_value(std::declval<Sink>())) return_type;

	difference_type shift = m_shift * adaptor_sub_dimension::prod;

apply_action:

	if (const auto actionRes = i_adaptor.perform_action(displace_action_tag{ static_cast<difference_type>(shift) }))
	{
		return return_type{ i_adaptor.forward_value(std::forward<Sink>(i_sink)) };
	}
	else
	{
		const displace_action_tag pendingAction = actionRes.recovery();

		if (pendingAction.displacement() < shift)
		{
			shift = -adaptor_dimension::prod + 2;

			goto apply_action;
		}
		else
		{
			return return_type{ stop_iteration };
		}
	}
}

TEMPLATE(typename Action)
REQUIRED(ANY_ACTION_TAGS_SUPPORTED(Action,begin_action_tag,forward_action_tag,backward_action_tag,displace_action_tag))
transpose_multi_dimensional_order_resolver::action transpose_multi_dimensional_order_resolver::operator()(const Action& i_action) const
{
	if constexpr (ACTION_TAGS_SUPPORTED_COND(Action,begin_action_tag))
	{
		return { 0 };
	}
	else if constexpr (ACTION_TAGS_SUPPORTED_COND(Action,forward_action_tag))
	{
		return { 1 };
	}
	else if constexpr (ACTION_TAGS_SUPPORTED_COND(Action,backward_action_tag))
	{
		return { -1 };
	}
	else
	{
		return { i_action.shift() };
	}
}

}
}
