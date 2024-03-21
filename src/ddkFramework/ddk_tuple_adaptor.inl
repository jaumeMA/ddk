
namespace ddk
{

template<size_t ... Indexs, typename ... T>
iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor, typename Sink>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_actionTag)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,sink_action_tag<Sink>> result_t;

	if (i_adaptor.m_currIndex < s_numTypes)
	{
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const tuple<T...>,tuple<T...>>::type tuple_t;
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const_reference,reference>::type return_t;
		typedef return_t(*funcType)(const sink_action_tag<Sink>&,tuple_t&);
		typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

		static const funcType funcTable[] = { &tuple_adaptor_t::template _get<Indexs,Sink,tuple_t,return_t> ... };

		return make_result<result_t>((*funcTable[i_adaptor.m_currIndex])(i_actionTag,std::forward<Adaptor>(i_adaptor).m_iterable));
	}
	else
	{
		return make_error<result_t>(i_actionTag);
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const begin_action_tag&)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag> result_t;

	i_adaptor.m_currIndex = 0;

	if (i_adaptor.m_currIndex < s_numTypes)
	{
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const tuple<T...>,tuple<T...>>::type tuple_t;
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const_reference,reference>::type return_t;
		typedef return_t(*funcType)(tuple_t&);
		typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

		static const funcType funcTable[] = { &tuple_adaptor_t::template _agnostic_get<Indexs,tuple_t,return_t> ... };

		return make_result<result_t>((*funcTable[i_adaptor.m_currIndex])(std::forward<Adaptor>(i_adaptor).m_iterable));
	}
	else
	{
		return make_error<result_t>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const end_action_tag&)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag> result_t;

	i_adaptor.m_currIndex = s_numTypes;

	if (i_adaptor.m_currIndex >= 0)
	{
		return make_result<result_t>(success);
	}
	else
	{
		return make_error<result_t>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const forward_action_tag&)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag> result_t;

	if (i_adaptor.m_currIndex < s_numTypes - 1)
	{
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const tuple<T...>,tuple<T...>>::type tuple_t;
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const_reference,reference>::type return_t;
		typedef return_t(*funcType)(tuple_t&);
		typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

		static const funcType funcTable[] = { &tuple_adaptor_t::template _agnostic_get<Indexs,tuple_t,return_t> ... };

		i_adaptor.m_currIndex++;

		return make_result<result_t>((*funcTable[i_adaptor.m_currIndex])(std::forward<Adaptor>(i_adaptor).m_iterable));
	}
	else
	{
		return make_error<result_t>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const backward_action_tag&)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag> result_t;

	if (i_adaptor.m_currIndex > 0)
	{
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const tuple<T...>,tuple<T...>>::type tuple_t;
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const_reference,reference>::type return_t;
		typedef return_t(*funcType)(tuple_t&);
		typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

		static const funcType funcTable[] = { &tuple_adaptor_t::template _agnostic_get<Indexs,tuple_t,return_t> ... };

		i_adaptor.m_currIndex--;

		return make_result<result_t>((*funcTable[i_adaptor.m_currIndex])(std::forward<Adaptor>(i_adaptor).m_iterable));
	}
	else
	{
		return make_error<result_t>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const displace_action_tag& i_action)
{
	typedef iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag> result_t;

	i_adaptor.m_currIndex += i_action.displacement();

	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_numTypes)
	{
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const tuple<T...>,tuple<T...>>::type tuple_t;
		typedef typename mpl::which_type<mpl::is_const<Adaptor>,const_reference,reference>::type return_t;
		typedef return_t(*funcType)(tuple_t&);
		typedef iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

		static const funcType funcTable[] = { &tuple_adaptor_t::template _agnostic_get<Indexs,tuple_t,return_t> ... };

		return make_result<result_t>((*funcTable[i_adaptor.m_currIndex])(std::forward<Adaptor>(i_adaptor).m_iterable));
	}
	else if (i_adaptor.m_currIndex >= s_numTypes)
	{
		const difference_type pendingShift = i_adaptor.m_currIndex - (s_numTypes - 1);

		i_adaptor.m_currIndex = s_numTypes - 1;

		return make_error<result_t>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = i_adaptor.m_currIndex;

		i_adaptor.m_currIndex = 0;

		return make_error<result_t>(pendingShift);
	}
}
template<size_t ... Indexs,typename ... T>
template<size_t Index,typename Sink,typename Tuple,typename Return>
Return iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_get(const sink_action_tag<Sink>& i_actionTag,Tuple& i_iterable)
{
	return i_actionTag(i_iterable.template get<Index>());
}
template<size_t ... Indexs,typename ... T>
template<size_t Index, typename Tuple, typename Return>
Return iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_agnostic_get(Tuple& i_iterable)
{
	return i_iterable.template get<Index>();
}

template<size_t ... Indexs, typename ... T>
iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::iterable_adaptor(const tuple<T...>& i_iterable)
: m_iterable(i_iterable)
{
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor, typename Sink>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_actionTag)
{
	typedef const_reference(*funcType)(const sink_action_tag<Sink>&,const tuple<T...>&);
	typedef iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>> tuple_adaptor_t;

	static const funcType funcTable[] = { &tuple_adaptor_t::template _const_get<Indexs> ... };

	if (i_adaptor.m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,sink_action_tag<Sink>>>((*funcTable[std::forward<Adaptor>(i_adaptor).m_currIndex])(i_actionTag,i_adaptor.m_iterable));
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,sink_action_tag<Sink>>>(i_actionTag);
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const begin_action_tag&)
{
	i_adaptor.m_currIndex = 0;

	if (i_adaptor.m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const end_action_tag&)
{
	i_adaptor.m_currIndex = s_numTypes;

	if (i_adaptor.m_currIndex >= 0)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const forward_action_tag&)
{
	if (i_adaptor.m_currIndex < s_numTypes - 1)
	{
		i_adaptor.m_currIndex++;

		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const backward_action_tag&)
{
	if (i_adaptor.m_currIndex > 0)
	{
		i_adaptor.m_currIndex--;

		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag>>();
	}
}
template<size_t ... Indexs,typename ... T>
template<typename Adaptor>
auto iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::perform_action(Adaptor&& i_adaptor, const displace_action_tag& i_action)
{
	typedef typename traits::difference_type difference_type;

	i_adaptor.m_currIndex += i_action.displacement();

	if (i_adaptor.m_currIndex >= 0 && i_adaptor.m_currIndex < s_numTypes)
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(success);
	}
	else if (i_adaptor.m_currIndex >= s_numTypes)
	{
		const difference_type pendingShift = i_adaptor.m_currIndex - (s_numTypes - 1);

		i_adaptor.m_currIndex = s_numTypes - 1;

		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(pendingShift);
	}
	else
	{
		const difference_type pendingShift = i_adaptor.m_currIndex;

		i_adaptor.m_currIndex = 0;

		return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag>>(pendingShift);
	}
}
template<size_t ... Indexs, typename ... T>
template<size_t Index,typename Sink>
typename iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::const_reference iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>::_const_get(const sink_action_tag<Sink>& i_actionTag,const tuple<T...>& i_iterable)
{
	return i_actionTag(i_iterable.template get<Index>());
}

}
