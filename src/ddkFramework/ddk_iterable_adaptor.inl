
namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Iterable>
template<typename Action>
iterable_adaptor_base<Iterable>::iterable_adaptor_base(Iterable& i_iterable, Action&& i_initialAction)
: m_iterable(i_iterable)
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const size_t shifting = i_initialAction.shifting() % iterableSize;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		suspend();
	}
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::reference iterable_adaptor_base<Iterable>::get_value()
{
	return *m_currIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::const_reference iterable_adaptor_base<Iterable>::get_value() const
{
	return *m_currIterator;
}
template<typename Iterable>
optional<typename iterable_adaptor_base<Iterable>::reference> iterable_adaptor_base<Iterable>::add_item(const_reference i_value)
{
	typedef typename Iterable::value_type value_type;

	iterator itNew = m_iterable.insert(m_currIterator,i_value);

	if(itNew != m_iterable.end()) return *itNew; else return none;
}
template<typename Iterable>
optional<typename iterable_adaptor_base<Iterable>::reference> iterable_adaptor_base<Iterable>::remove_item()
{
	iterator itNew = m_iterable.erase(m_currIterator);

	if(itNew != m_iterable.end()) return *itNew; else return none;
}
template<typename Iterable>
template<typename Action>
iterable_adaptor_base<const Iterable>::iterable_adaptor_base(const Iterable& i_iterable,Action&& i_initialAction)
: m_iterable(i_iterable)
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const size_t shifting = i_initialAction.shifting() % iterableSize;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		suspend();
	}
}
template<typename Iterable>
typename iterable_adaptor_base<const Iterable>::const_reference iterable_adaptor_base<const Iterable>::get_value() const
{
	return *m_currIterator;
}

template<typename Iterable>
optional<typename forward_iterable_adaptor<Iterable>::reference> forward_iterable_adaptor<Iterable>::next_value()
{
	if(++(this->m_currIterator) != m_iterable.end()) return *(this->m_currIterator); else return none;
}
template<typename Iterable>
optional<typename forward_iterable_adaptor<Iterable>::const_reference> forward_iterable_adaptor<Iterable>::next_value() const
{
	if(++(this->m_currIterator) != m_iterable.end()) return *(this->m_currIterator); else return none;
}

template<typename Iterable>
optional<typename bidirectional_iterable_adaptor<Iterable>::reference> bidirectional_iterable_adaptor<Iterable>::prev_value()
{
	if(--(this->m_currIterator) != m_iterable.end()) return *(this->m_currIterator); else return none;
}
template<typename Iterable>
optional<typename bidirectional_iterable_adaptor<Iterable>::const_reference> bidirectional_iterable_adaptor<Iterable>::prev_value() const
{
	if(--(this->m_currIterator) != m_iterable.end()) return *(this->m_currIterator); else return none;
}

template<typename Iterable>
optional<typename random_access_iterable_adaptor<Iterable>::reference> random_access_iterable_adaptor<Iterable>::shift_value(int i_shift)
{
	if((this->m_currIterator + i_shift) != m_iterable.end()) return *(this->m_currIterator + i_shift); else return none;
}
template<typename Iterable>
optional<typename random_access_iterable_adaptor<Iterable>::const_reference> random_access_iterable_adaptor<Iterable>::shift_value(int i_shift) const
{
	if((this->m_currIterator + i_shift) != m_iterable.end()) return *(this->m_currIterator + i_shift); else return none;
}

}
}
}