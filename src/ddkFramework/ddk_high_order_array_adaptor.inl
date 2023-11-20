
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
bool iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	return m_currentValue < high_order_array<T,ranks ...>::s_totalSize;

}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = high_order_array<T,ranks ...>::s_totalSize - 1;

	return m_currIndex > 0;
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		++m_currIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex > 0)
	{
		--m_currIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	const difference_type newIndex = m_currIndex + i_action.displacement();

	if (newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		m_currIndex = newIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
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
bool iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = high_order_array<T,ranks ...>::s_totalSize - 1;

	return m_currIndex > 0;
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		++m_currIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex > 0)
	{
		--m_currIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<const high_order_array<T,ranks...>>::perform_action(const displace_action_tag& i_action) const
{
	const difference_type newIndex = m_currIndex + i_action.displacement();

	if (newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		m_currIndex = newIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<const high_order_array<T,ranks...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
}

}
