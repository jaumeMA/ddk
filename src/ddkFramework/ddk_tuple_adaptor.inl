
namespace ddk
{

template<size_t ... Indexs, typename ... T>
iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(tuple<T...>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_next_value_in(Sink&& i_sink)
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_next_value_in(Sink&& i_sink) const
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_prev_value_in(Sink&& i_sink)
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_prev_value_in(Sink&& i_sink) const
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_shift_value_in(int i_shift,Sink&& i_sink)
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
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_shift_value_in(int i_shift,Sink&& i_sink) const
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
template<size_t ... Indexs, typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
void iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&, Sink&& i_sink)
{
	typedef void(*funcType)(Sink,tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
void iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&, Sink&& i_sink) const
{
	typedef void(*funcType)(Sink,const tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
void iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
void iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, const tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}


template<size_t ... Indexs, typename ... T>
iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(const tuple<T...>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_next_value_in(Sink&& i_sink)
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_next_value_in(Sink&& i_sink) const
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_prev_value_in(Sink&& i_sink)
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_prev_value_in(Sink&& i_sink) const
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		get(typename mpl::make_sequence<0,s_numTypes>::type{},std::forward<Sink>(i_sink));

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_shift_value_in(int i_shift,Sink&& i_sink)
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
template<size_t ... Indexs, typename ... T>
template<typename Sink>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_shift_value_in(int i_shift,Sink&& i_sink) const
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
template<size_t ... Indexs, typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&, Sink&& i_sink)
{
	typedef void(*funcType)(Sink,tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&, Sink&& i_sink) const
{
	typedef void(*funcType)(Sink,const tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, const tuple<T...>& i_iterable)
{
	i_sink(i_iterable.template get<Index>());
}

}
