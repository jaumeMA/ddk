#pragma once

#include "ddk_iterable_impl_interface.h"

namespace ddk
{
namespace detail
{

template<typename Traits, typename Iterable>
class iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;

    iterable_impl(Iterable& i_iterable);

private:
    void iterate_impl(const function<action(reference)>& i_try) override;
    void iterate_impl(const function<action(const_reference)>& i_try) const override;

    Iterable& m_iterable;
};

}
}

#include "ddk_iterable_impl.inl"
