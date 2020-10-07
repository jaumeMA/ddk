
namespace ddk
{
namespace detail
{

template<typename PublicTraits, typename PrivateTraits>
transformed_iterable_action_impl<PublicTraits,PrivateTraits>::transformed_iterable_action_impl(iterable_impl_shared_ref<private_iterable_base_traits> i_iterableRef, const function<action(private_action)>& i_transform)
: m_iterableRef(i_iterableRef)
, m_transform(i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_action_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    m_iterableRef->iterate_impl(make_composition(m_transform,i_try),i_initState);
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_action_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    m_iterableRef->iterate_impl(make_composition(m_transform,i_try),i_initState);
}
template<typename PublicTraits, typename PrivateTraits>
size_t transformed_iterable_action_impl<PublicTraits,PrivateTraits>::size() const
{
    return m_iterableRef->size();
}
template<typename PublicTraits, typename PrivateTraits>
bool transformed_iterable_action_impl<PublicTraits,PrivateTraits>::empty() const
{
    return m_iterableRef->empty();
}

}
}
