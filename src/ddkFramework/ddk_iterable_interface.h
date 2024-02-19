#pragma once

#include "ddk_iterable_supported_action.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_base
{
public:
    typedef Traits traits;
    typedef detail::const_iterable_traits<traits> const_traits;
    typedef supported_action<traits> action;
    typedef const_supported_action<traits> const_action;

    virtual ~iterable_base() = default;
    virtual void iterate(const action& i_initialAction) = 0;
    virtual void iterate(const const_action& i_initialAction) const = 0;
    virtual iterable_adaptor<type_erasure_iterable_impl<Traits>> deduce_owned_adaptor() = 0;
};

}
}