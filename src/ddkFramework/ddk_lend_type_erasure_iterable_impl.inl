#pragma once

namespace ddk
{
namespace detail
{

template<typename Traits>
lend_type_erasure_iterable_impl<Traits>::lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_base<Traits>>& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Traits>
template<typename Action>
void lend_type_erasure_iterable_impl<Traits>::iterate_impl(terse_endpoint i_try,const Action& i_initialAction)
{
	m_iterable->iterate(std::move(i_try),i_initialAction);
}
template<typename Traits>
template<typename Action>
void lend_type_erasure_iterable_impl<Traits>::iterate_impl(const_terse_endpoint i_try,const Action& i_initialAction) const
{
	m_iterable->iterate(std::move(i_try),i_initialAction);
}

}
}