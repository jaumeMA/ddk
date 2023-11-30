
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
typename iterable_adaptor_base<Iterable>::reference iterable_adaptor_base<Iterable>::get_value()
{
	return *this->m_currIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::const_reference iterable_adaptor_base<Iterable>::get_value() const
{
	return *this->m_currIterator;
}
template<typename Iterable>
template<typename Sink>
inline auto iterable_adaptor_base<Iterable>::forward_value(Sink&& i_sink)
{
	return ddk::eval(std::forward<Sink>(i_sink),*this->m_currIterator);
}
template<typename Iterable>
template<typename Sink>
inline auto iterable_adaptor_base<Iterable>::forward_value(Sink&& i_sink) const
{
	return ddk::eval(std::forward<Sink>(i_sink),*this->m_currIterator);
}
template<typename Iterable>
iterable_action_result<begin_action_tag> iterable_adaptor_base<Iterable>::perform_action(const begin_action_tag&) const
{
	m_currIterator = std::begin(m_iterable);

	if (m_currIterator != m_endIterator)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename Iterable>
iterable_action_result<last_action_tag> iterable_adaptor_base<Iterable>::perform_action(const last_action_tag&) const
{
	m_currIterator = std::prev(std::end(m_iterable));

	if (m_currIterator != m_endIterator)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename Iterable>
iterable_action_result<remove_action_tag> iterable_adaptor_base<Iterable>::perform_action(const remove_action_tag&)
{
	m_currIterator = m_iterable.erase(m_currIterator);

	if (m_currIterator != m_endIterator)
	{
		return success;
	}
	else
	{
		return {};
	}
}
template<typename Iterable>
iterable_action_result<add_action_tag<typename iterable_adaptor_base<Iterable>::value_type>> iterable_adaptor_base<Iterable>::perform_action(add_action_tag<value_type> i_action)
{
	m_iterable.insert(m_currIterator,std::move(i_action).extract());

	//here some check would be nice
	return success;
}

template<typename Iterable>
iterable_action_result<forward_action_tag> forward_iterable_adaptor<Iterable>::perform_action(const forward_action_tag&) const
{
	if (std::distance(this->m_currIterator,this->m_endIterator) > 1)
	{
		++this->m_currIterator;

		return success;
	}
	else
	{
		return {};
	}
}

template<typename Iterable>
bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor(Iterable& i_iterable)
: forward_iterable_adaptor<Iterable>(i_iterable)
, m_beginIterator(std::begin(i_iterable))
{
}
template<typename Iterable>
iterable_action_result<backward_action_tag> bidirectional_iterable_adaptor<Iterable>::perform_action(const backward_action_tag&) const
{
	if (std::distance(m_beginIterator,this->m_currIterator) > 0)
	{
		--this->m_currIterator;

		return success;
	}
	else
	{
		return {};
	}
}

template<typename Iterable>
iterable_action_result<displace_action_tag> random_access_iterable_adaptor<Iterable>::perform_action(const displace_action_tag& i_action) const
{
	const difference_type shift = i_action.displacement();

	if (shift > 0)
	{
		const difference_type maxDist = std::distance(this->m_currIterator,this->m_endIterator);

		if (shift < maxDist)
		{
			this->m_currIterator += shift;

			return success;
		}
		else
		{
			this->m_currIterator += maxDist;

			return { shift - maxDist };
		}
	}
	else
	{
		const difference_type maxDist = std::distance(this->m_beginIterator,this->m_currIterator);

		if (maxDist >= -shift)
		{
			this->m_currIterator -= shift;

			return success;
		}
		else
		{
			this->m_currIterator -= maxDist;

			return { shift + maxDist };
		}
	}
}

}
}
