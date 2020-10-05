#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class filtered_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
    typedef typename Traits::iterable_base_traits iterable_base_traits;

public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;

    filtered_iterable_impl(iterable_impl_shared_ref<iterable_base_traits> i_iterableRef, const function<bool(reference)>& i_filter);

private:
    void iterate_impl(const function<action(reference)>& i_try) override;
    void iterate_impl(const function<action(const_reference)>& i_try) const override;

    iterable_impl_shared_ref<iterable_base_traits> m_iterableRef;
    const function<bool(reference)> m_filter;
};

}
}

#include "ddk_filtered_iterable_impl.inl"
