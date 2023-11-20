#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterable_concepts.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class lend_type_erasure_iterable_impl : public iterable_impl_interface<typename Traits>
{
    friend inline auto deduce_adaptor(const lend_type_erasure_iterable_impl& i_iterableImpl)
    {
        return deduce_adaptor(*i_iterableImpl.m_iterable);
    }

public:
    typedef typename Traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;
    typedef supported_action<traits> action;
    typedef mpl::terse_callable<function<action(reference)>> terse_endpoint;
    typedef mpl::terse_callable<function<action(const_reference)>> const_terse_endpoint;

    lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_base<Traits>>& i_iterable);

    template<typename Action>
    void iterate_impl(terse_endpoint i_try,const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const_terse_endpoint i_try,const Action& i_initialAction) const;

private:
    lent_reference_wrapper<iterable_base<Traits>> m_iterable;
};
template<typename Traits>
lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_base<Traits>>&) -> lend_type_erasure_iterable_impl<Traits>;

}
}

#include "ddk_lend_type_erasure_iterable_impl.inl"