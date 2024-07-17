
namespace ddk
{
namespace detail
{

template<typename Adaptor>
template<typename Iterable>
iterable_adaptor_action<Adaptor,begin_action_tag>::iterable_adaptor_action(Iterable&& i_iterable)
: m_beginIterator(std::begin(std::forward<Iterable>(i_iterable)))
{
}
template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,begin_action_tag>::perform_action(AAdaptor&& i_adaptor, const begin_action_tag&)
{
	i_adaptor.m_currIterator = i_adaptor.m_beginIterator;

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (i_adaptor.m_currIterator != i_adaptor.m_endIterator)
		{
			return ddk::make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,begin_action_tag>>(*i_adaptor.m_currIterator);
		}
		else
		{
			return ddk::make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,begin_action_tag>>();
		}
	}
	else
	{
		return ddk::make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,begin_action_tag>>(*i_adaptor.m_currIterator);
	}
}

template<typename Adaptor>
template<typename Iterable>
iterable_adaptor_action<Adaptor,end_action_tag>::iterable_adaptor_action(Iterable&& i_iterable)
: m_endIterator(std::end(std::forward<Iterable>(i_iterable)))
{
}
template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,end_action_tag>::perform_action(AAdaptor&& i_adaptor, const end_action_tag&)
{
	i_adaptor.m_currIterator = i_adaptor.m_endIterator;

	return ddk::make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,end_action_tag>>(success);
}

template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,forward_action_tag>::perform_action(AAdaptor&& i_adaptor, const forward_action_tag&)
{
	++i_adaptor.m_currIterator;
	
	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (i_adaptor.m_currIterator != i_adaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,forward_action_tag>>(*i_adaptor.m_currIterator);
		}
		else
		{
			return make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,forward_action_tag>>();
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,forward_action_tag>>(*i_adaptor.m_currIterator);
	}
}

template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,backward_action_tag>::perform_action(AAdaptor&& i_adaptor, const backward_action_tag&)
{
	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,begin_action_tag>,Adaptor>::value)
	{
		if (i_adaptor.m_currIterator == i_adaptor.m_beginIterator)
		{
			return make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,backward_action_tag>>();
		}
		else
		{
			--i_adaptor.m_currIterator;

			return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,backward_action_tag>>(*i_adaptor.m_currIterator);
		}
	}
	else
	{
		--i_adaptor.m_currIterator;

		return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,backward_action_tag>>(*i_adaptor.m_currIterator);
	}
}

template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,displace_action_tag>::perform_action(AAdaptor&& i_adaptor, const displace_action_tag& i_action)
{
	typedef typename detail::adaptor_traits<AAdaptor>::difference_type difference_type;

	const difference_type shift = i_action.displacement();

	if (shift > 0)
	{
		if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
		{
			const difference_type maxDist = std::distance(i_adaptor.m_currIterator,i_adaptor.m_endIterator);

			if (shift < maxDist)
			{
				i_adaptor.m_currIterator += shift;

				return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(*i_adaptor.m_currIterator);
			}
			else
			{
				return make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(shift - maxDist);
			}
		}
		else
		{
			i_adaptor.m_currIterator += shift;

			return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(*i_adaptor.m_currIterator);
		}
	}
	else
	{
		difference_type maxDist = shift + 1;

		if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,begin_action_tag>,Adaptor>::value)
		{
			const difference_type maxDist = std::distance(i_adaptor.m_beginIterator,i_adaptor.m_currIterator);

			if (maxDist >= -shift)
			{
				i_adaptor.m_currIterator -= shift;

				return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(*i_adaptor.m_currIterator);
			}
			else
			{
				return make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(shift - maxDist);
			}
		}
		else
		{
			i_adaptor.m_currIterator -= shift;

			return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,displace_action_tag>>(*i_adaptor.m_currIterator);
		}
	}
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,remove_action_tag>::perform_action(Adaptor& i_adaptor, const remove_action_tag&)
{
	i_adaptor.m_currIterator = i_adaptor.m_iterable.erase(i_adaptor.m_currIterator);

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (i_adaptor.m_currIterator != i_adaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,remove_action_tag>>(success);
		}
		else
		{
			return make_error<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,remove_action_tag>>();
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,remove_action_tag>>(success);
	}
}

template<typename Adaptor, typename T>
constexpr auto iterable_adaptor_action<Adaptor,add_action_tag<T>>::perform_action(Adaptor& i_adaptor, add_action_tag<T> i_action)
{
	i_adaptor.m_currIterator = i_adaptor.m_iterable.insert(i_adaptor.m_currIterator,std::move(i_action).extract());

	//here some check would be nice
	return make_result<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,add_action_tag<T>>>(*i_adaptor.m_currIterator);
}

template<typename Adaptor,typename T>
template<typename AAdaptor, typename Sink>
constexpr auto iterable_adaptor_action<Adaptor,agnostic_sink_action_tag<T>>::perform_action(AAdaptor&& i_adaptor, const sink_action_tag<Sink>& i_sink)
{
	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (i_adaptor.m_currIterator != i_adaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,sink_action_tag<Sink>>>(*i_adaptor.m_currIterator);
		}
		else
		{
			return make_error<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,sink_action_tag<Sink>>>(i_sink);
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,sink_action_tag<Sink>>>(i_sink(*i_adaptor.m_currIterator));
	}
}

template<typename Adaptor>
template<typename AAdaptor>
constexpr auto iterable_adaptor_action<Adaptor,size_action_tag>::perform_action(AAdaptor&& i_adaptor, const size_action_tag&)
{
	return make_result<iterable_action_tag_result<detail::adaptor_traits<AAdaptor>,size_action_tag>>(std::size(i_adaptor.m_iterable));
}

template<typename Iterable,typename ... IterableActions>
iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>::iterable_adaptor_actions(Iterable& i_iterable)
: iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions>(i_iterable) ...
, m_iterable(i_iterable)
{
}

}
}
