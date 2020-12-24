
namespace ddk
{

template<typename ... T>
tuple_adaptor<T...>::tuple_adaptor(tuple<T...>& i_iterable,const ddk::iter::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
	
	get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<typename ... T>
tuple_adaptor<T...>::~tuple_adaptor()
{
	m_currValue.template destroy<variant<typename embedded_type<T>::ref_type ...>>();
}
template<typename ... T>
typename tuple_adaptor<T...>::reference tuple_adaptor<T...>::get_value() noexcept
{
	//return m_currValue.template get<variant<typename embedded_type<T>::ref_type ...>>();

	return get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<typename ... T>
typename tuple_adaptor<T...>::const_reference tuple_adaptor<T...>::get_value() const noexcept
{
	//return m_currValue.template get<variant<typename embedded_type<T>::cref_type ...>>();

	return get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::reference> tuple_adaptor<T...>::next_value() noexcept
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::ref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::const_reference> tuple_adaptor<T...>::next_value() const noexcept
{
	if(m_currIndex < s_numTypes)
	{
		m_currIndex++;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::cref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::reference> tuple_adaptor<T...>::prev_value() noexcept
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::ref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::const_reference> tuple_adaptor<T...>::prev_value() const noexcept
{
	if(m_currIndex >= 0)
	{
		m_currIndex--;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::cref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::reference> tuple_adaptor<T...>::shift_value(int i_shift) noexcept
{
	size_t newIndex = m_currIndex + i_shift;

	if(0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::ref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
ddk::optional<typename tuple_adaptor<T...>::const_reference> tuple_adaptor<T...>::shift_value(int i_shift) const noexcept
{
	size_t newIndex = m_currIndex + i_shift;

	if(0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		return get(typename mpl::make_sequence<0,s_numTypes>::type{});

		//return m_currValue.template get<variant<typename embedded_type<T>::cref_type ...>>();
	}
	else
	{
		return none;
	}
}
template<typename ... T>
bool tuple_adaptor<T...>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<typename ... T>
template<size_t ... Indexs>
typename tuple_adaptor<T...>::reference tuple_adaptor<T...>::get(const mpl::sequence<Indexs...>&)
{
	typedef reference(*funcType)(tuple<T...>&,cached_value_t&);
	typedef tuple_adaptor<T...> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	return (*funcTable[m_currIndex])(m_iterable,m_currValue);
}
template<typename ... T>
template<size_t ... Indexs>
typename tuple_adaptor<T...>::const_reference tuple_adaptor<T...>::get(const mpl::sequence<Indexs...>&) const
{
	typedef const_reference(*funcType)(const tuple<T...>&,cached_value_t&);
	typedef tuple_adaptor<T...> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::_get<Indexs> ... };

	return (*funcTable[m_currIndex])(m_iterable,m_currValue);
}
template<typename ... T>
template<size_t Index>
typename tuple_adaptor<T...>::reference tuple_adaptor<T...>::_get(tuple<T...>& i_iterable, cached_value_t& o_currValue)
{
	o_currValue.template construct<variant<typename embedded_type<T>::ref_type ...>>(i_iterable.template get<Index>());

	return { i_iterable.template get<Index>() };
}
template<typename ... T>
template<size_t Index>
typename tuple_adaptor<T...>::const_reference tuple_adaptor<T...>::_get(const tuple<T...>& i_iterable, cached_value_t& o_currValue)
{
	o_currValue.template construct<variant<typename embedded_type<T>::ref_type ...>>(i_iterable.template get<Index>());

	return { i_iterable.template get<Index>() };
}

}