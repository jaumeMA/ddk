
namespace ddk
{

template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,begin_action_tag> union_iterable_action<begin_action_tag>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(0))
	{
		return i_adaptor.adaptor_base::perform_action(begin_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,begin_action_tag>>();
	}
}

template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,begin_next_iterable> union_iterable_action<begin_next_iterable>::apply(Adaptor&& i_adaptor)
{
apply_begin_next_iterable:
	if (i_adaptor.set_current_iterable_index(i_adaptor.get_current_iterable_index() + 1))
	{
		if (auto beginRes = i_adaptor.adaptor_base::perform_action(begin_action_tag{}))
		{
			return beginRes;
		}
		else
		{
			goto apply_begin_next_iterable;
		}
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,begin_next_iterable>>();
	}
}

template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,end_action_tag> union_iterable_action<end_action_tag>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(i_adaptor.s_numTypes - 1))
	{
		return i_adaptor.adaptor_base::perform_action(end_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,end_action_tag>>();
	}
}

template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,end_prev_iterable> union_iterable_action<end_prev_iterable>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(i_adaptor.get_current_iterable_index() - 1))
	{
		return i_adaptor.adaptor_base::perform_action(end_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,end_prev_iterable>>();
	}
}

template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,size_action_tag> union_iterable_action<size_action_tag>::apply(Adaptor&& i_adaptor)
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;

	return _apply(std::forward<Adaptor>(i_adaptor),typename mpl::make_sequence<0,adaptor_t::s_numTypes>::type{});
}
template<typename Adaptor,size_t ... Indexs>
union_iterable_action_result<Adaptor,size_action_tag> union_iterable_action<size_action_tag>::_apply(Adaptor&& i_adaptor,const mpl::sequence<Indexs...>&)
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;
	const tuple<iterable_action_tag_result<typename adaptor_t::template nth_adaptor<Indexs>::const_traits,size_action_tag>...> adaptorResults = { i_adaptor.adaptor_base::template get_adaptor<Indexs>().perform_action(size_action_tag{}) ... };
	const bool adaptorBools[mpl::num_ranks<Indexs...>] = { static_cast<bool>(adaptorResults.template get<Indexs>()) ... };

	if ((adaptorBools[Indexs] && ...))
	{
		return make_result<union_iterable_action_result<Adaptor,size_action_tag>>((adaptorResults.template get<Indexs>().get() + ...));
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,size_action_tag>>();
	}
}

template<typename ActionTag>
TEMPLATE(typename AActionTag)
REQUIRED(IS_CONSTRUCTIBLE(ActionTag,AActionTag))
union_iterable_action<ActionTag>::union_iterable_action(AActionTag&& i_actionTag)
: m_actionTag(std::forward<AActionTag>(i_actionTag))
{
}
template<typename ActionTag>
union_iterable_action<ActionTag>::union_iterable_action(ActionTag&& i_actionTag)
: m_actionTag(std::move(i_actionTag))
{
}
template<typename ActionTag>
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,ActionTag> union_iterable_action<ActionTag>::apply(Adaptor&& i_adaptor)
{
	return i_adaptor.adaptor_base::perform_action(std::move(m_actionTag));
}
template<typename ActionTag>
const ActionTag& union_iterable_action<ActionTag>::action() const
{
	return m_actionTag;
}

}