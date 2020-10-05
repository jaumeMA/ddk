#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_sync_executor_impl.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_interface
{
public:
    typedef typename Traits::action action;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef Traits traits;

    virtual ~iterable_interface() = default;

    virtual reference resolve_action(const action&) = 0;
    virtual const_reference resolve_action(const action&) const = 0;

private:

};

template<typename Traits>
using iterable_lent_ref = lent_reference_wrapper<iterable_interface<Traits>>;
template<typename Traits>
using iterable_const_lent_ref = lent_reference_wrapper<const iterable_interface<Traits>>;
template<typename Traits>
using iterable_lent_ptr = lent_pointer_wrapper<iterable_interface<Traits>>;
template<typename Traits>
using iterable_const_lent_ptr = lent_pointer_wrapper<const iterable_interface<Traits>>;

}

template<typename Iterable, typename IIterable>
inline Iterable make_iterable(IIterable&& i_iterable);

}

#include "ddk_iterable_interface.inl"
