
namespace ddk
{

template<typename T,size_t ... ranks>
iterable_adaptor<high_order_array<T,ranks...>>::iterable_adaptor(high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const sink_action_tag<Sink>& i_actionTag)
{
	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_actionTag(m_iterable.at(m_currIndex)));
	}
	else
	{
		return make_error<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const sink_action_tag<Sink>& i_actionTag) const
{
	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_actionTag(m_iterable.at(m_currIndex)));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const end_action_tag&) const
{
	m_currIndex = s_totalSize;

	if (m_currIndex > 0)
	{
		return make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,end_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = m_currIndex - 1;

	if (newIndex >= 0)
	{
		m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename traits::difference_type difference_type;

	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
	}
	else if(m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = m_currIndex - (s_totalSize - 1);

		m_currIndex = s_totalSize - 1;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
}

template<typename T,size_t ... ranks>
iterable_adaptor<const high_order_array<T,ranks...>>::iterable_adaptor(const high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const sink_action_tag<Sink>& i_actionTag) const
{
	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		i_actionTag(m_iterable.at(m_currIndex));

		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const end_action_tag&) const
{
	m_currIndex = s_totalSize;

	if (m_currIndex >= 0)
	{
		return make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,end_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	typedef typename traits::difference_type difference_type;

	const difference_type newIndex = m_currIndex - 1;

	if (newIndex >= 0)
	{
		m_currIndex = newIndex;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename traits::difference_type difference_type;

	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
	}
	else if (m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = m_currIndex - (s_totalSize - 1);

		m_currIndex = s_totalSize - 1;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
}

}
