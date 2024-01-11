
namespace ddk
{
namespace detail
{

TEMPLATE(typename Adaptor,typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> forward_order_resolver::operator()(Adaptor&& i_adaptor,ActionTag&& i_action) const
{
	return i_adaptor.perform_action(std::forward<ActionTag>(i_action));
}

TEMPLATE(typename Adaptor,typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> backward_order_resolver::operator()(Adaptor&& i_adaptor, ActionTag&& i_action) const
{
	return i_adaptor.perform_action(std::forward<ActionTag>(i_action));
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,last_action_tag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag> backward_order_resolver::operator()(Adaptor&& i_adaptor,begin_action_tag&& i_action) const
{
	if (const auto actionRes = i_adaptor.perform_action(last_action_tag{}))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,begin_action_tag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,last_action_tag> backward_order_resolver::operator()(Adaptor&& i_adaptor,last_action_tag&& i_action) const
{
	if (const auto actionRes = i_adaptor.perform_action(begin_action_tag{}))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,last_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,last_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag> backward_order_resolver::operator()(Adaptor&& i_adaptor, forward_action_tag&& i_action) const
{
	if (const auto actionRes = i_adaptor.perform_action(backward_action_tag{}))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag> backward_order_resolver::operator()(Adaptor&& i_adaptor, backward_action_tag&& i_action) const
{
	if (const auto actionRes = i_adaptor.perform_action(forward_action_tag{}))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,displace_action_tag))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag> backward_order_resolver::operator()(Adaptor&& i_adaptor, displace_action_tag&& i_action) const
{
	if (const auto actionRes = i_adaptor.perform_action(displace_action_tag{ -i_action.displacement() }))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(-actionRes.error().recovery().displacement());
	}
}

TEMPLATE(typename Adaptor,typename ActionTag)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> transpose_multi_dimensional_order_resolver::operator()(Adaptor&& i_adaptor,ActionTag&& i_action) const
{
	return i_adaptor.perform_action(std::forward<ActionTag>(i_action));
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag> transpose_multi_dimensional_order_resolver::operator()(Adaptor&& i_adaptor, forward_action_tag&& i_action) const
{
	typedef typename mpl::remove_qualifiers<Adaptor>::traits adaptor_traits;
	typedef typename adaptor_traits::difference_type difference_type;

	if (const auto actionRes = perform_action(std::forward<Adaptor>(i_adaptor),difference_type{ 1 }))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag> transpose_multi_dimensional_order_resolver::operator()(Adaptor&& i_adaptor, backward_action_tag&& i_action) const
{
	typedef typename mpl::remove_qualifiers<Adaptor>::traits adaptor_traits;
	typedef typename adaptor_traits::difference_type difference_type;

	if (const auto actionRes = perform_action(std::forward<Adaptor>(i_adaptor),difference_type{ -1 }))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>();
	}
}
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,displace_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag> transpose_multi_dimensional_order_resolver::operator()(Adaptor&& i_adaptor, displace_action_tag&& i_action) const
{
	return perform_action(std::forward<Adaptor>(i_adaptor),i_action.displacement());
}
template<typename Adaptor>
auto transpose_multi_dimensional_order_resolver::perform_action(Adaptor&& i_adaptor,typename mpl::remove_qualifiers<Adaptor>::traits::difference_type i_displacement) const
{
	typedef typename mpl::remove_qualifiers<Adaptor> adaptor_t;
	typedef typename adaptor_t::dimension_t adaptor_dimension;
	typedef typename adaptor_t::traits adaptor_traits;
	typedef typename adaptor_traits::difference_type difference_type;
	typedef typename adaptor_dimension::template drop<0> adaptor_sub_dimension;

	difference_type shift = i_displacement * adaptor_sub_dimension::prod;

apply_action:

	if (const auto actionRes = i_adaptor.perform_action(displace_action_tag{ static_cast<difference_type>(shift) }))
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(actionRes);
	}
	else
	{
		const displace_action_tag pendingAction = actionRes.error().recovery();

		if (pendingAction.displacement() < shift)
		{
			shift = -static_cast<difference_type>(adaptor_dimension::prod) + 2;

			goto apply_action;
		}
		else
		{
			return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(pendingAction.displacement());
		}
	}
}

}
}
