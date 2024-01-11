
namespace ddk
{
namespace detail
{

template<typename Iterable>
iterable_adaptor_base<Iterable>::iterable_adaptor_base(Iterable& i_iterable)
: m_iterable(i_iterable)
, m_currIterator(std::end(m_iterable))
, m_endIterator(std::end(i_iterable))
{
}
template<typename Iterable>
template<typename Sink>
auto iterable_adaptor_base<Iterable>::perform_action(const sink_action_tag<Sink>& i_sink)
{
	if (m_currIterator != m_endIterator)
	{
		return make_result<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_sink(*this->m_currIterator));
	}
	else
	{
		return make_error<iterable_action_tag_result<traits,sink_action_tag<Sink>>>(i_sink);
	}
}
template<typename Iterable>
template<typename Sink>
auto iterable_adaptor_base<Iterable>::perform_action(const sink_action_tag<Sink>& i_sink) const
{
	if (m_currIterator != m_endIterator)
	{
		return make_result<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_sink(*this->m_currIterator));
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,sink_action_tag<Sink>>>(i_sink);
	}
}
template<typename Iterable>
auto iterable_adaptor_base<Iterable>::perform_action(const begin_action_tag&) const
{
	m_currIterator = std::begin(m_iterable);

	if (m_currIterator != m_endIterator)
	{
		return ddk::make_result<iterable_action_tag_result<const_traits,begin_action_tag>>(success);
	}
	else
	{
		return ddk::make_error<iterable_action_tag_result<const_traits,begin_action_tag>>();
	}
}
template<typename Iterable>
auto iterable_adaptor_base<Iterable>::perform_action(const last_action_tag&) const
{
	m_currIterator = std::prev(std::end(m_iterable));

	if (m_currIterator != m_endIterator)
	{
		return make_result<iterable_action_tag_result<const_traits,last_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,last_action_tag>>();
	}
}
template<typename Iterable>
auto iterable_adaptor_base<Iterable>::perform_action(const remove_action_tag&)
{
	m_currIterator = m_iterable.erase(m_currIterator);

	if (m_currIterator != m_endIterator)
	{
		return make_result<iterable_action_tag_result<traits,remove_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<traits,remove_action_tag>>();
	}
}
template<typename Iterable>
auto iterable_adaptor_base<Iterable>::perform_action(add_action_tag<value_type> i_action)
{
	typedef typename Iterable::value_type value_type;

	m_iterable.insert(m_currIterator,std::move(i_action).extract());

	//here some check would be nice
	return make_result<iterable_action_tag_result<traits,add_action_tag<value_type>>>(success);
}

template<typename Iterable>
auto forward_iterable_adaptor<Iterable>::perform_action(const forward_action_tag&) const
{
	if (std::distance(this->m_currIterator,this->m_endIterator) > 1)
	{
		++this->m_currIterator;

		return make_result<iterable_action_tag_result<const_traits,forward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,forward_action_tag>>();
	}
}

template<typename Iterable>
bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor(Iterable& i_iterable)
: forward_iterable_adaptor<Iterable>(i_iterable)
, m_beginIterator(std::begin(i_iterable))
{
}
template<typename Iterable>
auto bidirectional_iterable_adaptor<Iterable>::perform_action(const backward_action_tag&) const
{
	if (std::distance(m_beginIterator,this->m_currIterator) > 0)
	{
		--this->m_currIterator;

		return make_result<iterable_action_tag_result<const_traits,backward_action_tag>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<const_traits,backward_action_tag>>();
	}
}

template<typename Iterable>
auto random_access_iterable_adaptor<Iterable>::perform_action(const displace_action_tag& i_action) const
{
	typedef typename traits::difference_type difference_type;

	const difference_type shift = i_action.displacement();

	if (shift > 0)
	{
		const difference_type maxDist = std::distance(this->m_currIterator,this->m_endIterator);

		if (shift < maxDist)
		{
			this->m_currIterator += shift;

			return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
		}
		else
		{
			this->m_currIterator += maxDist;

			return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(shift - maxDist);
		}
	}
	else
	{
		const difference_type maxDist = std::distance(this->m_beginIterator,this->m_currIterator);

		if (maxDist >= -shift)
		{
			this->m_currIterator -= shift;

			return make_result<iterable_action_tag_result<const_traits,displace_action_tag>>(success);
		}
		else
		{
			this->m_currIterator -= maxDist;

			return make_error<iterable_action_tag_result<const_traits,displace_action_tag>>(shift - maxDist);
		}
	}
}

}
}
