
namespace ddk
{

constexpr union_iterable_action<begin_action_tag>::union_iterable_action(begin_action_tag&&)
{
}
constexpr begin_action_tag union_iterable_action<begin_action_tag>::action() const
{
	return {};
}
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,begin_action_tag> union_iterable_action<begin_action_tag>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(0))
	{
		typedef typename mpl::remove_qualifiers<Adaptor>::adaptor_base adaptor_base;

		return adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),begin_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,begin_action_tag>>();
	}
}

constexpr union_iterable_action<begin_next_iterable>::union_iterable_action(begin_next_iterable&&)
{
}
constexpr begin_next_iterable union_iterable_action<begin_next_iterable>::action() const
{
	return {};
}
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,begin_next_iterable> union_iterable_action<begin_next_iterable>::apply(Adaptor&& i_adaptor)
{
	while(true)
	{
		if (i_adaptor.set_current_iterable_index(i_adaptor.get_current_iterable_index() + 1))
		{
			typedef typename mpl::remove_qualifiers<Adaptor>::adaptor_base adaptor_base;

			if (auto beginRes = adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),begin_action_tag{}))
			{
				return beginRes;
			}
			else
			{
				continue;
			}
		}
		else
		{
			return make_error<union_iterable_action_result<Adaptor,begin_next_iterable>>();
		}
	}
}

constexpr union_iterable_action<end_action_tag>::union_iterable_action(end_action_tag&&)
{
}
constexpr end_action_tag union_iterable_action<end_action_tag>::action() const
{
	return {};
}
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,end_action_tag> union_iterable_action<end_action_tag>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(i_adaptor.s_numTypes - 1))
	{
		typedef typename mpl::remove_qualifiers<Adaptor>::adaptor_base adaptor_base;

		return adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),end_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,end_action_tag>>();
	}
}

constexpr union_iterable_action<end_prev_iterable>::union_iterable_action(end_prev_iterable&&)
{
}
constexpr end_prev_iterable union_iterable_action<end_prev_iterable>::action() const
{
	return {};
}
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,end_prev_iterable> union_iterable_action<end_prev_iterable>::apply(Adaptor&& i_adaptor)
{
	if (i_adaptor.set_current_iterable_index(i_adaptor.get_current_iterable_index() - 1))
	{
		typedef typename mpl::remove_qualifiers<Adaptor>::adaptor_base adaptor_base;

		adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),end_action_tag{}).dismiss();

		return adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),backward_action_tag{});
	}
	else
	{
		return make_error<union_iterable_action_result<Adaptor,end_prev_iterable>>();
	}
}

constexpr union_iterable_action<size_action_tag>::union_iterable_action(const size_action_tag&)
{
}
constexpr size_action_tag union_iterable_action<size_action_tag>::action() const
{
	return {};
}
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,size_action_tag> union_iterable_action<size_action_tag>::apply(Adaptor&& i_adaptor)
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;

	return _apply(std::forward<Adaptor>(i_adaptor),typename mpl::make_sequence<0,adaptor_t::s_numTypes>::type{});
}
template<typename Adaptor,size_t ... Indexs>
constexpr union_iterable_action_result<Adaptor,size_action_tag> union_iterable_action<size_action_tag>::_apply(Adaptor&& i_adaptor,const mpl::sequence<Indexs...>&)
{
	typedef mpl::remove_qualifiers<Adaptor> adaptor_t;
	const tuple<iterable_action_tag_result<detail::adaptor_traits<typename adaptor_t::template nth_adaptor<Indexs>>,size_action_tag>...> adaptorResults{ adaptor_t::template nth_adaptor<Indexs>::perform_action(std::forward<Adaptor>(i_adaptor).template get_adaptor<Indexs>(),size_action_tag{}) ... };

	if ((static_cast<bool>(adaptorResults.template get<Indexs>()) && ...))
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
constexpr union_iterable_action<ActionTag>::union_iterable_action(AActionTag&& i_actionTag)
: m_actionTag(std::forward<AActionTag>(i_actionTag))
{
}
template<typename ActionTag>
constexpr union_iterable_action<ActionTag>::union_iterable_action(ActionTag&& i_actionTag)
: m_actionTag(std::move(i_actionTag))
{
}
template<typename ActionTag>
template<typename Adaptor>
constexpr union_iterable_action_result<Adaptor,ActionTag> union_iterable_action<ActionTag>::apply(Adaptor&& i_adaptor)
{
	typedef typename mpl::remove_qualifiers<Adaptor>::adaptor_base adaptor_base;

	return adaptor_base::perform_action(std::forward<Adaptor>(i_adaptor),std::move(m_actionTag));
}
template<typename ActionTag>
constexpr const ActionTag& union_iterable_action<ActionTag>::action() const
{
	return m_actionTag;
}

}