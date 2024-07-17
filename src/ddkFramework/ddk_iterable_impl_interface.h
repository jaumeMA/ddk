//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_impl_interface
{
public:
    typedef Traits traits;

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

template<typename Traits>
Traits deduce_iterable_traits(const iterable_impl_interface<Traits>&);

}
}

#include "ddk_iterable_impl_interface.inl"
