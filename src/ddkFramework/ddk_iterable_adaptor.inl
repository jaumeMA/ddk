
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
bool iterable_adaptor_base<Iterable>::valid() const noexcept
{
	return m_currIterator != m_endIterator;
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::perform_action(const begin_action_tag&) const
{
	m_currIterator = std::begin(m_iterable);

	return m_currIterator != m_endIterator;
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::perform_action(const last_action_tag&) const
{
	m_currIterator = std::prev(std::end(m_iterable));

	return m_currIterator != m_endIterator;
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::perform_action(const remove_action_tag&)
{
	m_iterable.erase(m_currIterator);

	return true;
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::perform_action(add_action_tag<value_type> i_action)
{
	m_iterable.insert(m_currIterator,std::move(i_action).extract());

	return true;
}

template<typename Iterable>
bool forward_iterable_adaptor<Iterable>::perform_action(const forward_action_tag&) const
{
	if (std::distance(this->m_currIterator,this->m_endIterator) > 1)
	{
		++this->m_currIterator;

		return true;
	}
	else
	{
		return false;
	}
}

template<typename Iterable>
bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor(Iterable& i_iterable)
: forward_iterable_adaptor<Iterable>(i_iterable)
, m_beginIterator(std::begin(i_iterable))
{
}
template<typename Iterable>
bool bidirectional_iterable_adaptor<Iterable>::perform_action(const backward_action_tag&) const
{
	if (std::distance(m_beginIterator,this->m_currIterator) > 0)
	{
		--this->m_currIterator;

		return true;
	}
	else
	{
		return false;
	}
}

template<typename Iterable>
bool random_access_iterable_adaptor<Iterable>::perform_action(const displace_action_tag& i_action) const
{
	const difference_type shift = i_action.displacement();

	if (shift > 0)
	{
		if (std::distance(this->m_currIterator,this->m_endIterator) > shift)
		{
			this->m_currIterator += shift;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (std::distance(this->m_beginIterator,this->m_currIterator) >= -shift)
		{
			this->m_currIterator -= shift;

			return true;
		}
		else
		{
			return false;
		}
	}
}

}
}
