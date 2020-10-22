
namespace ddk
{
namespace detail
{

template<typename Function>
iterable_transform<Function>::iterable_transform(const Function& i_transform)
: m_transform(i_transform)
{
}
template<typename Function>
Function iterable_transform<Function>::get_transform() const
{
	return m_transform;
}

template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::transformed_iterable_impl(iterable_impl_shared_ref<private_iterable_base_traits> i_iterableRef, const function<Return(Type)>& i_transform)
: m_iterableRef(i_iterableRef)
, m_transform(i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
void transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    m_iterableRef->iterate_impl(make_composition(i_try,m_transform),i_initState);
}
template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
void transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    m_iterableRef->iterate_impl(make_composition(make_function(make_composition(i_try,m_transform)),make_function([](const_reference i_ref) -> reference { return const_cast<reference>(i_ref); })),i_initState);
}
template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
size_t transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::size() const
{
    return m_iterableRef->size();
}
template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
bool transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::empty() const
{
    return m_iterableRef->empty();
}

}
}
