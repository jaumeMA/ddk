#pragma once

#include "ddk_iterable_defs.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class lend_type_erasure_iterable_impl : public iterable_impl_interface<Traits>
{
public:
    typedef typename Traits traits;

    lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_base<Traits>>& i_iterable);

    template<typename Action>
    void iterate_impl(const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const Action& i_initialAction) const;

private:
    lent_reference_wrapper<iterable_base<Traits>> m_iterable;
};
template<typename Traits>
lend_type_erasure_iterable_impl(const lent_reference_wrapper<iterable_base<Traits>>&) -> lend_type_erasure_iterable_impl<Traits>;

}
}

#include "ddk_lend_type_erasure_iterable_impl.inl"