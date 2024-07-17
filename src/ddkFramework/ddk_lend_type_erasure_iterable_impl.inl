#pragma once

namespace ddk
{
namespace detail
{

template<typename Traits>
lend_type_erasure_iterable_impl<Traits>::lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_interface<Traits>>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Traits>
template<typename Action>
void lend_type_erasure_iterable_impl<Traits>::iterate_impl(const Action& i_initialAction)
{
	m_iterable->iterate(i_initialAction);
}
template<typename Traits>
template<typename Action>
void lend_type_erasure_iterable_impl<Traits>::iterate_impl(const Action& i_initialAction) const
{
	m_iterable->iterate(i_initialAction);
}

}
}