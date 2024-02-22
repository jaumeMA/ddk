
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
constexpr auto iterable_adaptor_action<Adaptor,begin_action_tag>::perform_action(const begin_action_tag&) const
{
	typedef typename Adaptor::const_traits const_traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);
	
	thisAdaptor.m_currIterator = m_beginIterator;

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator != thisAdaptor.m_endIterator)
		{
			return ddk::make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
		}
		else
		{
			return ddk::make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
		}
	}
	else
	{
		return ddk::make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
}

template<typename Adaptor>
template<typename Iterable>
iterable_adaptor_action<Adaptor,end_action_tag>::iterable_adaptor_action(Iterable&& i_iterable)
: m_endIterator(std::end(std::forward<Iterable>(i_iterable)))
{
}
template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,end_action_tag>::perform_action(const end_action_tag&) const
{
	typedef typename Adaptor::const_traits const_traits;

	static_cast<const Adaptor&>(*this).m_currIterator = m_endIterator;

	return ddk::make_result<iterable_action_tag_result<const_traits,end_action_tag>>(success);
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,forward_action_tag>::perform_action(const forward_action_tag&) const
{
	typedef typename Adaptor::const_traits const_traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	++thisAdaptor.m_currIterator;
	
	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator != thisAdaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
		}
		else
		{
			return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,backward_action_tag>::perform_action(const backward_action_tag&) const
{
	typedef typename Adaptor::const_traits const_traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,begin_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator == thisAdaptor.m_beginIterator)
		{
			return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
		}
		else
		{
			--thisAdaptor.m_currIterator;

			return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
		}
	}
	else
	{
		--thisAdaptor.m_currIterator;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,displace_action_tag>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename Adaptor::const_traits const_traits;
	typedef typename const_traits::difference_type difference_type;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);
	const difference_type shift = i_action.displacement();

	if (shift > 0)
	{
		if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
		{
			const difference_type maxDist = std::distance(thisAdaptor.m_currIterator,thisAdaptor.m_endIterator);

			if (shift < maxDist)
			{
				thisAdaptor.m_currIterator += shift;

				return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
			}
			else
			{
				return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(shift - maxDist);
			}
		}
		else
		{
			thisAdaptor.m_currIterator += shift;

			return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
		}
	}
	else
	{
		difference_type maxDist = shift + 1;

		if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,begin_action_tag>,Adaptor>::value)
		{
			const difference_type maxDist = std::distance(thisAdaptor.m_beginIterator,thisAdaptor.m_currIterator);

			if (maxDist >= -shift)
			{
				thisAdaptor.m_currIterator -= shift;

				return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
			}
			else
			{
				return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(shift - maxDist);
			}
		}
		else
		{
			thisAdaptor.m_currIterator -= shift;

			return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
		}
	}
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,remove_action_tag>::perform_action(const remove_action_tag&) const
{
	typedef typename Adaptor::traits traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	thisAdaptor.m_currIterator = thisAdaptor.m_iterable.erase(thisAdaptor.m_currIterator);

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator != thisAdaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<traits,remove_action_tag>>(success);
		}
		else
		{
			return make_error<iterable_action_tag_result<traits,remove_action_tag>>();
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<traits,remove_action_tag>>(success);
	}
}

template<typename Adaptor, typename T>
constexpr auto iterable_adaptor_action<Adaptor,add_action_tag<T>>::perform_action(add_action_tag<T> i_action) const
{
	typedef typename Adaptor::traits traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	thisAdaptor.m_currIterator = thisAdaptor.m_iterable.insert(thisAdaptor.m_currIterator,std::move(i_action).extract());

	//here some check would be nice
	return make_result<iterable_action_tag_result<traits,add_action_tag<T>>>(success);
}

template<typename Adaptor,typename T>
template<typename Sink>
constexpr auto iterable_adaptor_action<Adaptor,agnostic_sink_action_tag<T>>::perform_action(const sink_action_tag<Sink>& i_sink)
{
	typedef typename Adaptor::traits traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator != thisAdaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_sink(*thisAdaptor.m_currIterator));
		}
		else
		{
			return make_error<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_sink);
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_sink(*thisAdaptor.m_currIterator));
	}
}
template<typename Adaptor,typename T>
template<typename Sink>
constexpr auto iterable_adaptor_action<Adaptor,agnostic_sink_action_tag<T>>::perform_action(const sink_action_tag<Sink>& i_sink) const
{
	typedef typename Adaptor::const_traits const_traits;

	const Adaptor& thisAdaptor = static_cast<const Adaptor&>(*this);

	if constexpr (std::is_base_of<iterable_adaptor_action<Adaptor,end_action_tag>,Adaptor>::value)
	{
		if (thisAdaptor.m_currIterator != thisAdaptor.m_endIterator)
		{
			return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_sink(*thisAdaptor.m_currIterator));
		}
		else
		{
			return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_sink);
		}
	}
	else
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_sink(*thisAdaptor.m_currIterator));
	}
}

template<typename Adaptor>
constexpr auto iterable_adaptor_action<Adaptor,size_action_tag>::perform_action(const size_action_tag&) const
{
	typedef typename Adaptor::const_traits const_traits;

	return make_result<iterable_action_tag_result<const_traits,size_action_tag>>(std::size(static_cast<const Adaptor&>(*this).m_iterable));
}

template<typename Iterable,typename ... IterableActions>
iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>::iterable_adaptor_actions(Iterable& i_iterable)
: iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions>(i_iterable) ...
, m_iterable(i_iterable)
{
}

}
}
