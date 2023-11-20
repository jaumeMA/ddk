#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_impl_interface
{
public:
    typedef Traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;

    virtual ~iterable_impl_interface() = default;
};

template<typename Traits>
using iterable_impl_dist_ref = distributed_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_dist_ref = distributed_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_dist_ptr = distributed_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_dist_ptr = distributed_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename Traits>
using iterable_impl_lent_ref = lent_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ref = lent_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_lent_ptr = lent_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ptr = lent_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename IterableImpl, typename ... Args>
inline distributed_reference_wrapper<IterableImpl> make_iterable_impl(Args&& ... i_args);

}
}

#include "ddk_iterable_impl_interface.inl"
