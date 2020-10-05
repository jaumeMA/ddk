#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"

namespace ddk
{
namespace detail
{

template<typename Iterable, typename ... Iterables>
class union_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
public:
    union_iterable_impl(const Iterable& i_iterable, const Iterables& ... i_iterables);

private:
    void iterate_impl(const function<action(reference)>& i_try) override;
    void iterate_impl(const function<action(const_reference)>& i_try) const override;

    tuple<Iterable,Iterables...> m_iterables;
};

}
}

