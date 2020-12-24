
namespace ddk
{

template<typename T,size_t ... ranks>
high_order_array_adaptor<T,ranks...>::high_order_array_adaptor(high_order_array<T,ranks...>& i_iterable,const ddk::iter::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	shift_value(i_initialAction.shifting());
}
template<typename T,size_t ... ranks>
typename high_order_array_adaptor<T,ranks...>::reference high_order_array_adaptor<T,ranks...>::get_value() noexcept
{
	return m_iterable.at(m_currIndex);
}
template<typename T,size_t ... ranks>
typename high_order_array_adaptor<T,ranks...>::const_reference high_order_array_adaptor<T,ranks...>::get_value() const noexcept
{
	return m_iterable.at(m_currIndex);
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::reference> high_order_array_adaptor<T,ranks...>::next_value() noexcept
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		return m_iterable.at(++m_currIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::const_reference> high_order_array_adaptor<T,ranks...>::next_value() const noexcept
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		return m_iterable.at(++m_currIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::reference> high_order_array_adaptor<T,ranks...>::prev_value() noexcept
{
	if(m_currIndex > 0)
	{
		return m_iterable.at(--m_currIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::const_reference> high_order_array_adaptor<T,ranks...>::prev_value() const noexcept
{
	if(m_currIndex > 0)
	{
		return m_iterable.at(--m_currIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::reference> high_order_array_adaptor<T,ranks...>::shift_value(int i_shift) noexcept
{
	const size_t newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		return m_iterable.at(m_currIndex = newIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
ddk::optional<typename high_order_array_adaptor<T,ranks...>::const_reference> high_order_array_adaptor<T,ranks...>::shift_value(int i_shift) const noexcept
{
	const size_t newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		return m_iterable.at(m_currIndex = newIndex);
	}
	else
	{
		return none;
	}
}
template<typename T,size_t ... ranks>
bool high_order_array_adaptor<T,ranks...>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
}

}