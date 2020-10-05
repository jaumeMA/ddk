
namespace ddk
{
namespace detail
{

template<typename PublicTraits, typename PrivateTraits>
transformed_iterable_impl<PublicTraits,PrivateTraits>::transformed_iterable_impl(iterable_impl_shared_ref<private_iterable_base_traits> i_iterableRef, const function<reference(private_reference)>& i_transform)
: m_iterableRef(i_iterableRef)
, m_transform(i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(reference)>& i_try)
{
    m_iterableRef->iterate_impl(make_composition(i_try,m_transform));
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(const_reference)>& i_try) const
{
    m_iterableRef->iterate_impl(make_composition(i_try,m_transform));
}

}
}
