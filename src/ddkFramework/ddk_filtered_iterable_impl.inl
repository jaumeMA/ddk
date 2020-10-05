
namespace ddk
{
namespace detail
{

template<typename Traits>
filtered_iterable_impl<Traits>::filtered_iterable_impl(iterable_impl_shared_ref<iterable_base_traits> i_iterableRef, const function<bool(reference)>& i_filter)
: m_iterableRef(i_iterableRef)
, m_filter(i_filter)
{
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(reference)>& i_try)
{
    m_iterableRef->iterate_impl([i_try,this](reference i_value) -> action { return (m_filter(i_value)) ? eval(i_try,i_value) : iter::go_next_place; });
}
template<typename Traits>
void filtered_iterable_impl<Traits>::iterate_impl(const function<action(const_reference)>& i_try) const
{
    m_iterableRef->iterate_impl([i_try,this](const_reference i_value) -> action { return (m_filter(i_value)) ? eval(i_try,i_value) : iter::go_next_place; });
}

}
}
