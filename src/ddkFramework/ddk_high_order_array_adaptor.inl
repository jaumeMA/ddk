
namespace ddk
{

template<typename T,size_t ... ranks>
high_order_array_adaptor<T,ranks...>::high_order_array_adaptor(high_order_array<T,ranks...>& i_iterable,const ddk::iter::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_next_value_in(Sink&& i_sink)
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink(m_iterable.at(++m_currIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_next_value_in(Sink&& i_sink) const
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink(m_iterable.at(++m_currIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_prev_value_in(Sink&& i_sink)
{
	if(m_currIndex > 0)
	{
		i_sink(m_iterable.at(--m_currIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_prev_value_in(Sink&& i_sink) const
{
	if(m_currIndex > 0)
	{
		i_sink(m_iterable.at(--m_currIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_shift_value_in(int i_shift,Sink&& i_sink)
{
	const size_t newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink(m_iterable.at(m_currIndex = newIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
bool high_order_array_adaptor<T,ranks...>::forward_shift_value_in(int i_shift,Sink&& i_sink) const
{
	const size_t newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink(m_iterable.at(m_currIndex = newIndex));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T,size_t ... ranks>
bool high_order_array_adaptor<T,ranks...>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
}

}