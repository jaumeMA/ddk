
namespace ddk
{

template<typename T,size_t ... ranks>
iterable_adaptor<high_order_array<T,ranks...>>::iterable_adaptor(high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
template<typename Adaptor, typename Sink>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_actionTag)
{
	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,sink_action_tag<Sink>>>(i_actionTag(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex)));
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const begin_action_tag&)
{
	i_adaptor.m_currIndex = 0;

	if (i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const end_action_tag&)
{
	i_adaptor.m_currIndex = s_totalSize;

	if (i_adaptor.m_currIndex > 0)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const forward_action_tag&)
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = i_adaptor.m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		i_adaptor.m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const backward_action_tag&)
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = i_adaptor.m_currIndex - 1;

	if (newIndex >= 0)
	{
		i_adaptor.m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const displace_action_tag& i_action)
{
	typedef typename traits::difference_type difference_type;

	i_adaptor.m_currIndex += i_action.displacement();

	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else if(i_adaptor.m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = i_adaptor.m_currIndex - (s_totalSize - 1);

		i_adaptor.m_currIndex = s_totalSize - 1;

		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = i_adaptor.m_currIndex;

		i_adaptor.m_currIndex = 0;

		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(pendingShift);
	}
}

template<typename T,size_t ... ranks>
iterable_adaptor<const high_order_array<T,ranks...>>::iterable_adaptor(const high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
template<typename Adaptor, typename Sink>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_actionTag)
{
	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_actionTag(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex)));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const begin_action_tag&)
{
	i_adaptor.m_currIndex = 0;

	if (i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const end_action_tag&)
{
	i_adaptor.m_currIndex = s_totalSize;

	if (i_adaptor.m_currIndex >= 0)
	{
		return make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,end_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const forward_action_tag&)
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = i_adaptor.m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		i_adaptor.m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const backward_action_tag&)
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = i_adaptor.m_currIndex - 1;

	if (newIndex >= 0)
	{
		i_adaptor.m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
template<typename Adaptor>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(Adaptor&& i_adaptor, const displace_action_tag& i_action)
{
	typedef typename traits::difference_type difference_type;

	i_adaptor.m_currIndex += i_action.displacement();

	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(std::forward<Adaptor>(i_adaptor).m_iterable.at(i_adaptor.m_currIndex));
	}
	else if (i_adaptor.m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = i_adaptor.m_currIndex - (s_totalSize - 1);

		i_adaptor.m_currIndex = s_totalSize - 1;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = i_adaptor.m_currIndex;

		i_adaptor.m_currIndex = 0;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
}

}
