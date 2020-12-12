
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
, m_endIterator(std::end(i_iterable))
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const int targetShift = i_initialAction.shifting();
		const int shifting = (targetShift < 0) ? static_cast<int>(iterableSize) + targetShift : targetShift;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		m_currIterator = m_endIterator;
	}
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::valid() const noexcept
{
	return m_currIterator != m_endIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::reference iterable_adaptor_base<Iterable>::get_value() noexcept
{
	return *m_currIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::const_reference iterable_adaptor_base<Iterable>::get_value() const noexcept
{
	return *m_currIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::pointer iterable_adaptor_base<Iterable>::add_item(const_reference i_value) noexcept
{
	typedef typename Iterable::value_type value_type;

	iterator itNew = m_iterable.insert(m_currIterator,i_value);

	if (itNew != this->m_endIterator) return &*itNew; else return nullptr;
}
template<typename Iterable>
typename iterable_adaptor_base<Iterable>::pointer iterable_adaptor_base<Iterable>::remove_item() noexcept
{
	iterator itNew = m_iterable.erase(m_currIterator);

	if(itNew != this->m_endIterator) return &*itNew; else return nullptr;
}
template<typename Iterable>
template<typename Action>
iterable_adaptor_base<const Iterable>::iterable_adaptor_base(const Iterable& i_iterable,Action&& i_initialAction)
: m_iterable(i_iterable)
, m_endIterator(std::end(i_iterable))
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const int targetShift = i_initialAction.shifting();
		const int shifting = (targetShift < 0) ? static_cast<int>(iterableSize) + targetShift : targetShift;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		m_currIterator = m_endIterator;
	}
}
template<typename Iterable>
bool iterable_adaptor_base<const Iterable>::valid() const noexcept
{
	return m_currIterator != m_endIterator;
}
template<typename Iterable>
typename iterable_adaptor_base<const Iterable>::const_reference iterable_adaptor_base<const Iterable>::get_value() const noexcept
{
	return *m_currIterator;
}

template<typename Iterable>
typename forward_iterable_adaptor<Iterable>::pointer forward_iterable_adaptor<Iterable>::next_value() noexcept
{
	if(++(this->m_currIterator) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
}
template<typename Iterable>
typename forward_iterable_adaptor<Iterable>::const_pointer forward_iterable_adaptor<Iterable>::next_value() const noexcept
{
	if(++(this->m_currIterator) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
}

template<typename Iterable>
template<typename Action>
bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor(Iterable& i_iterable,Action&& i_initialAction)
: forward_iterable_adaptor<Iterable>(i_iterable,i_initialAction)
, m_endReverseIterator(std::rend(i_iterable))
{
}
template<typename Iterable>
typename bidirectional_iterable_adaptor<Iterable>::pointer bidirectional_iterable_adaptor<Iterable>::prev_value() noexcept
{
	if(std::make_reverse_iterator((this->m_currIterator)--) != m_endReverseIterator) return &*(this->m_currIterator); else return nullptr;
}
template<typename Iterable>
typename bidirectional_iterable_adaptor<Iterable>::const_pointer bidirectional_iterable_adaptor<Iterable>::prev_value() const noexcept
{
	return (std::make_reverse_iterator((this->m_currIterator)--) != m_endReverseIterator) return &*(this->m_currIterator); else return nullptr;
}

template<typename Iterable>
typename random_access_iterable_adaptor<Iterable>::pointer random_access_iterable_adaptor<Iterable>::shift_value(int i_shift) noexcept
{
	switch(i_shift)
	{
	case 1:
		if(++(this->m_currIterator) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
	case -1:
		if(std::make_reverse_iterator((this->m_currIterator)--) != this->m_endReverseIterator) return &*(this->m_currIterator); else return nullptr;
	default:
		if((this->m_currIterator += i_shift) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
	}
}
template<typename Iterable>
typename random_access_iterable_adaptor<Iterable>::const_pointer random_access_iterable_adaptor<Iterable>::shift_value(int i_shift) const noexcept
{
	switch(i_shift)
	{
	case 1:
		if(++(this->m_currIterator) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
	case -1:
		if(std::make_reverse_iterator((this->m_currIterator)--) != this->m_endReverseIterator) return &*(this->m_currIterator); else return nullptr;
	default:
		if((this->m_currIterator += i_shift) != this->m_endIterator) return &*(this->m_currIterator); else return nullptr;
	}
}

}
}
}