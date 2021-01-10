
namespace ddk
{

template<typename ... T>
tuple_adaptor<T...>::tuple_adaptor(tuple<T...>& i_iterable,const ddk::iter::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_next_value_in(Sink&& i_sink) noexcept
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{});

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_next_value_in(Sink&& i_sink) const noexcept
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{});

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_prev_value_in(Sink&& i_sink) noexcept
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{});

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_prev_value_in(Sink&& i_sink) const noexcept
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{});

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_shift_value_in(int i_shift,Sink&& i_sink) noexcept
{
	size_t newIndex = m_currIndex + i_shift;

	if(0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
template<typename Sink>
bool tuple_adaptor<T...>::forward_shift_value_in(int i_shift,Sink&& i_sink) const noexcept
{
	size_t newIndex = m_currIndex + i_shift;

	if(0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<typename ... T>
bool tuple_adaptor<T...>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<typename ... T>
template<typename Sink, size_t ... Indexs>
void tuple_adaptor<T...>::get(const mpl::sequence<Indexs...>&, Sink&& i_sink)
{
	typedef void(*funcType)(Sink,tuple<T...>&);
	typedef tuple_adaptor<T...> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<typename ... T>
template<typename Sink, size_t ... Indexs>
void tuple_adaptor<T...>::get(const mpl::sequence<Indexs...>&, Sink&& i_sink) const
{
	typedef void(*funcType)(Sink,const tuple<T...>&);
	typedef tuple_adaptor<T...> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<typename ... T>
template<size_t Index,typename Sink>
void tuple_adaptor<T...>::_get(Sink&& i_sink, tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}
template<typename ... T>
template<size_t Index,typename Sink>
void tuple_adaptor<T...>::_get(Sink&& i_sink, const tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}

}