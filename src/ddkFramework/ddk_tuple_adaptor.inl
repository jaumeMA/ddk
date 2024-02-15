
namespace ddk
{

template<size_t ... Indexs, typename ... T>
iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const sink_action_tag<Sink>& i_actionTag)
{
	typedef reference(*funcType)(const sink_action_tag<Sink>&,tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	if (m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<traits,sink_action_tag<Sink>>>((*funcTable[m_currIndex])(i_actionTag,m_iterable));
	}
	else
	{
		return make_error<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const sink_action_tag<Sink>& i_actionTag) const
{
	typedef const_reference(*funcType)(const sink_action_tag<Sink>&,const tuple<T...>&);
	typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs> ... };

	if (m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>((*funcTable[m_currIndex])(i_actionTag,m_iterable));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const end_action_tag&) const
{
	m_currIndex = s_numTypes;

	if (m_currIndex >= 0)
	{
		return make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,end_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < s_numTypes - 1)
	{
		m_currIndex++;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex > 0)
	{
		m_currIndex--;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename traits::difference_type difference_type;

	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
	}
	else if (m_currIndex >= s_numTypes)
	{
		const difference_type pendingShift = m_currIndex - (s_numTypes - 1);

		m_currIndex = s_numTypes - 1;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
}
template<size_t ... Indexs,typename ... T>
template<size_t Index,typename Sink>
typename iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::reference iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(const sink_action_tag<Sink>& i_actionTag,tuple<T...>& i_iterable)
{
	return i_actionTag(i_iterable.template get<Index>());
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
typename iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::const_reference iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(const sink_action_tag<Sink>& i_actionTag,const tuple<T...>& i_iterable)
{
	return i_actionTag(i_iterable.template get<Index>());
}

template<size_t ... Indexs, typename ... T>
iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(const tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
template<typename Sink>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const sink_action_tag<Sink>& i_actionTag) const
{
	typedef const_reference(*funcType)(const sink_action_tag<Sink>&,const tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _const_get<Indexs> ... };

	if (m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>((*funcTable[m_currIndex])(i_actionTag,m_iterable));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const begin_action_tag&) const
{
	m_currIndex = 0;

	if (m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const end_action_tag&) const
{
	m_currIndex = s_numTypes;

	if (m_currIndex >= 0)
	{
		return make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,end_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const forward_action_tag&) const
{
	if (m_currIndex < s_numTypes - 1)
	{
		m_currIndex++;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const backward_action_tag&) const
{
	if (m_currIndex > 0)
	{
		m_currIndex--;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename traits::difference_type difference_type;

	m_currIndex += i_action.displacement();

	if (m_currIndex >= 0 && m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
	}
	else if (m_currIndex >= s_numTypes)
	{
		const difference_type pendingShift = m_currIndex - (s_numTypes - 1);

		m_currIndex = s_numTypes - 1;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = m_currIndex;

		m_currIndex = 0;

		return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(pendingShift);
	}
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
typename iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::const_reference iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_const_get(const sink_action_tag<Sink>& i_actionTag,const tuple<T...>& i_iterable)
{
	return i_actionTag(i_iterable.template get<Index>());
}

}
