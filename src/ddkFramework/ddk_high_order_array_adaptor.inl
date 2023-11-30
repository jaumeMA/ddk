
namespace ddk
{

template<typename T,size_t ... ranks>
iterable_adaptor<high_order_array<T,ranks...>>::iterable_adaptor(high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::get_value()
{
	return m_iterable.at(m_currIndex);
}
template<typename T,size_t ... ranks>
auto iterable_adaptor<high_order_array<T,ranks...>>::get_value() const
{
	return m_iterable.at(m_currIndex);
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<high_order_array<T,ranks...>>::forward_value(Sink&& i_sink)
{
	return ddk::eval(std::forward<Sink>(i_sink),m_iterable.at(m_currIndex));
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<high_order_array<T,ranks...>>::forward_value(Sink&& i_sink) const
{
	return ddk::eval(std::forward<Sink>(i_sink),m_iterable.at(m_currIndex));
}
template<typename T,size_t ... ranks>
iterable_action_result<begin_action_tag> iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_totalSize)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<last_action_tag> iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = s_totalSize - 1;

	if (m_currIndex > 0)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<forward_action_tag> iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	const difference_type newIndex = m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		m_currIndex = newIndex;

		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<backward_action_tag> iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	const difference_type newIndex = m_currIndex - 1;

	if (newIndex >= 0)
	{
		m_currIndex = newIndex;

		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<displace_action_tag> iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return success;
	}
	else if(m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = m_currIndex - (s_totalSize - 1);

		m_currIndex = s_totalSize - 1;

		return { pendingShift };
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return { pendingShift };
	}
}

template<typename T,size_t ... ranks>
iterable_adaptor<const high_order_array<T,ranks...>>::iterable_adaptor(const high_order_array<T,ranks...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename T,size_t ... ranks>
template<typename Sink>
auto iterable_adaptor<const high_order_array<T,ranks...>>::forward_value(Sink&& i_sink) const
{
	return ddk::eval(std::forward<Sink>(i_sink),m_iterable.at(m_currIndex));
}
template<typename T,size_t ... ranks>
iterable_action_result<begin_action_tag> iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_totalSize)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<last_action_tag> iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = s_totalSize - 1;

	if (m_currIndex >= 0)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<forward_action_tag> iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	const difference_type newIndex = m_currIndex + 1;

	if (newIndex < s_totalSize)
	{
		m_currIndex = newIndex;

		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<backward_action_tag> iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	const difference_type newIndex = m_currIndex - 1;

	if (newIndex >= 0)
	{
		m_currIndex = newIndex;

		return success;
	}
	else
	{
		return {};
	}
}
template<typename T,size_t ... ranks>
iterable_action_result<displace_action_tag> iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_totalSize)
	{
		return success;
	}
	else if (m_currIndex >= s_totalSize)
	{
		const difference_type pendingShift = m_currIndex - (s_totalSize - 1);

		m_currIndex = s_totalSize - 1;

		return { pendingShift };
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return { pendingShift };
	}
}

}
