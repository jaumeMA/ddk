
namespace ddk
{

template<size_t ... Indexs, typename ... T>
iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get_value()
{
	return get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get_value() const
{
	return get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_value(Sink&& i_sink)
{
	return get(std::forward<Sink>(i_sink),typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_value(Sink&& i_sink) const
{
	return get(std::forward<Sink>(i_sink),typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	return valid();
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = s_numTypes - 1;

	return valid();
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < s_numTypes)
	{
		m_currIndex++;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex >= 0)
	{
		m_currIndex--;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const displace_action_tag& i_action, difference_type i_shift) const
{
	const difference_type newIndex = m_currIndex + i_shift;

	if (0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
bool iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::valid() const
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(Sink&& i_sink,const mpl::sequence<IIndexs...>&)
{
	typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
	typedef return_type(*funcType)(Sink&&,tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	return (*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(Sink&& i_sink,const mpl::sequence<IIndexs...>&) const
{
	typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
	typedef return_type(*funcType)(Sink&&,const tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _const_get<Indexs> ... };

	return (*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, tuple<T...>& i_iterable)
{
	return ddk::eval(std::forward<Sink>(i_sink),i_iterable.template get<Index>());
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_const_get(Sink&& i_sink, const tuple<T...>& i_iterable)
{
	return ddk::eval(std::forward<Sink>(i_sink),i_iterable.template get<Index>());
}
template<size_t ... Indexs,typename ... T>
template<size_t ... IIndexs>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&)
{
	typedef reference(*funcType)(tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	return (*funcTable[m_currIndex])(m_iterable);
}
template<size_t ... Indexs,typename ... T>
template<size_t ... IIndexs>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&) const
{
	typedef const_reference(*funcType)(const tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _const_get<Indexs> ... };

	return (*funcTable[m_currIndex])(m_iterable);
}
template<size_t ... Indexs,typename ... T>
template<size_t Index>
typename iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::reference iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(tuple<T...>& i_iterable)
{
	return i_iterable.template get<Index>();
}
template<size_t ... Indexs,typename ... T>
template<size_t Index>
typename iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::const_reference iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_const_get(const tuple<T...>& i_iterable)
{
	return i_iterable.template get<Index>();
}


template<size_t ... Indexs, typename ... T>
iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(const tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get_value() const
{
	return get(typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::forward_value(Sink&& i_sink) const
{
	return get(std::forward<Sink>(i_sink),typename mpl::make_sequence<0,s_numTypes>::type{});
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	return valid();
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const last_action_tag&) const
{
	m_currIndex = s_numTypes - 1;

	return valid();
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < s_numTypes)
	{
		m_currIndex++;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex >= 0)
	{
		m_currIndex--;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs,typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const displace_action_tag& i_actio, difference_type i_shift) const
{
	const difference_type newIndex = m_currIndex + i_shift;

	if (0 <= newIndex && newIndex < s_numTypes)
	{
		m_currIndex = newIndex;

		return true;
	}
	else
	{
		return false;
	}
}
template<size_t ... Indexs, typename ... T>
bool iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::valid() const
{
	return 0 <= m_currIndex && m_currIndex < s_numTypes;
}
template<size_t ... Indexs, typename ... T>
template<typename Sink, size_t ... IIndexs>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&, Sink&& i_sink) const
{
	typedef typename mpl::aqcuire_callable_return_type<Sink>::type return_type;
	typedef return_type(*funcType)(Sink,const tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
void iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(Sink&& i_sink, const tuple<T...>& i_iterable)
{
	return ddk::eval(std::forward<Sink>(i_sink),i_iterable.template get<Index>());
}
template<size_t ... Indexs,typename ... T>
template<size_t ... IIndexs>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::get(const mpl::sequence<IIndexs...>&) const
{
	typedef const_reference(*funcType)(Sink,const tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	(*funcTable[m_currIndex])(std::forward<Sink>(i_sink),m_iterable);
}
template<size_t ... Indexs,typename ... T>
template<size_t Index>
typename iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::const_reference iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(const tuple<T...>& i_iterable)
{
	return i_iterable.template get<Index>();
}

}
