
namespace ddk
{
namespace detail
{

template<typename Traits,bool Const>
iterable_value<Traits,Const>::iterable_value(reference i_value, const iterable_interface_lent_ref& i_iterableRef)
: m_value(&i_value)
, m_iterableRef(i_iterableRef)
{
}
template<typename Traits,bool Const>
template<typename TTraits>
iterable_value<Traits,Const>::iterable_value(reference i_value, const lent_pointer_wrapper<const detail::iterable_interface<TTraits>>& i_iterableRef, typename std::enable_if<(Const==true) && std::is_convertible<typename TTraits::reference,typename Traits::reference>::value>::type*)
: m_value(&i_value)
, m_iterableRef(reinterpret_lent_cast<detail::iterable_interface_type<Traits,Const>>(i_iterableRef))
{
}
template<typename Traits,bool Const>
template<typename TTraits>
iterable_value<Traits,Const>::iterable_value(reference i_value, const lent_pointer_wrapper<detail::iterable_interface<TTraits>>& i_iterableRef, typename std::enable_if<(Const==false) && std::is_convertible<typename TTraits::reference,typename Traits::reference>::value>::type*)
: m_value(&i_value)
, m_iterableRef(reinterpret_lent_cast<detail::iterable_interface_type<Traits,Const>>(i_iterableRef))
{
}
template<typename Traits,bool Const>
typename iterable_value<Traits,Const>::reference iterable_value<Traits,Const>::operator*()
{
    return *m_value;
}
template<typename Traits,bool Const>
typename iterable_value<Traits,Const>::const_reference iterable_value<Traits,Const>::operator*() const
{
    return *m_value;
}
template<typename Traits,bool Const>
typename iterable_value<Traits,Const>::pointer iterable_value<Traits,Const>::operator->()
{
    return m_value;
}
template<typename Traits,bool Const>
typename iterable_value<Traits,Const>::const_pointer iterable_value<Traits,Const>::operator->() const
{
    return m_value;
}
template<typename Traits,bool Const>
bool iterable_value<Traits,Const>::operator==(const_reference i_value) const
{
    return *m_value == i_value;
}
template<typename Traits,bool Const>
bool iterable_value<Traits,Const>::operator!=(const_reference i_value) const
{
    return *m_value == i_value;
}

}

template<typename T>
template<typename TT>
const_forwarded_value<T>::const_forwarded_value(const_forwarded_value<TT>&& other)
: const_iterable_value<detail::forward_iterable_traits<T>>(static_cast<typename detail::forward_iterable_traits<T>::reference>(other.m_value),other.m_iterableRef)
{
}

template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(forwarded_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(static_cast<typename detail::forward_iterable_traits<T>::reference>(*other.m_value),other.m_iterableRef)
{
}

template<typename T>
template<typename TT>
const_bidirectional_value<T>::const_bidirectional_value(const_bidirectional_value<TT>&& other)
: const_iterable_value<detail::bidirectional_iterable_traits<T>>(static_cast<typename detail::bidirectional_iterable_traits<T>::reference>(*other.m_value),other.m_iterableRef)
{
}

template<typename T>
template<typename TT>
bidirectional_value<T>::bidirectional_value(bidirectional_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(static_cast<typename detail::bidirectional_iterable_traits<T>::reference>(*other.m_value),other.m_iterableRef)
{
}

template<typename T>
template<typename TT>
const_random_accessed_value<T>::const_random_accessed_value(const_random_accessed_value<TT>&& other)
: const_iterable_value<detail::random_access_iterable_traits<T>>(static_cast<typename detail::random_access_iterable_traits<T>::reference>(*other.m_value),other.m_iterableRef)
{
}

template<typename T>
template<typename TT>
random_accessed_value<T>::random_accessed_value(random_accessed_value<TT>&& other)
: iterable_value<detail::random_access_iterable_traits<T>>(static_cast<typename detail::random_access_iterable_traits<T>::reference>(*other.m_value),other.m_iterableRef)
{
}

template<typename Value, typename ... Args>
Value make_iterable_value(Args&& ... i_args)
{
    Value res(std::forward<Args>(i_args) ...);

    return std::move(res);
}

}
