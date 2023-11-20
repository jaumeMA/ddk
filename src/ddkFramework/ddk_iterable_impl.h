#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Traits, typename Iterable>
class iterable_impl : public iterable_impl_interface<Traits>, protected iterable_visitor<Iterable>
{
    friend inline iterable_adaptor<Iterable> deduce_adaptor(const iterable_impl& i_iterable)
    {
        return { i_iterable.m_adaptor };
    }

public:
    typedef Traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;

    iterable_impl(Iterable& i_iterable);

    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference))
    void iterate_impl(Function&& i_try, const Action& i_initialAction);
    TEMPLATE(typename Function,typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference))
    void iterate_impl(Function&& i_try, const Action& i_initialAction) const;
};

}
}

#include "ddk_iterable_impl.inl"