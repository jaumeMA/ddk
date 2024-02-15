
namespace ddk
{

template<typename Adaptor>
union_iterable_action_result<Adaptor,begin_action_tag> union_iterable_action<begin_action_tag>::apply(Adaptor&& i_adaptor)
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
union_iterable_action_result<Adaptor,begin_next_iterable> union_iterable_action<begin_next_iterable>::apply(Adaptor&& i_adaptor)
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
union_iterable_action_result<Adaptor,end_action_tag> union_iterable_action<end_action_tag>::apply(Adaptor&& i_adaptor)
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
union_iterable_action_result<Adaptor,end_prev_iterable> union_iterable_action<end_prev_iterable>::apply(Adaptor&& i_adaptor)
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
union_iterable_action_result<Adaptor,ActionTag> union_iterable_action<ActionTag>::apply(Adaptor&& i_adaptor)
{
	return i_adaptor.adaptor_base::perform_action(std::move(m_actionTag));
}
template<typename ActionTag>
const ActionTag& union_iterable_action<ActionTag>::action() const
{
	return m_actionTag;
}

}