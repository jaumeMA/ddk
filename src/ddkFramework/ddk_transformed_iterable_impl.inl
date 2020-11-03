
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
void transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::iterate_impl(const function<action(reference)>& i_try, const iter::shift_action& i_initialAction)
{
	m_iterableRef->iterate_impl(make_composition(i_try,make_function([this](private_reference i_value) -> Return { return eval(m_transform,i_value); })),i_initialAction);
}
template<typename PublicTraits, typename PrivateTraits, typename Return, typename Type>
void transformed_iterable_impl<PublicTraits,PrivateTraits,Return,Type>::iterate_impl(const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction) const
{
	m_iterableRef->iterate_impl(make_composition(i_try,make_function([this](private_const_reference i_value) -> Return { return eval(m_transform,const_cast<private_reference>(i_value)); })),i_initialAction);
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
