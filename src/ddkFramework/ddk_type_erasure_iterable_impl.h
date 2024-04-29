//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lend_type_erasure_iterable_impl.h"
#include "ddk_iterable_concepts.h"
#include "ddk_iterable_traits_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class type_erasure_iterable_impl : public iterable_impl_interface<Traits>
{
    friend inline auto lend(const type_erasure_iterable_impl& i_iterable)
    {
        return lend_type_erasure_iterable_impl{lend(i_iterable.m_iterable)};
    }
    friend inline auto deduce_adaptor(type_erasure_iterable_impl& i_iterable)
    {
        return i_iterable.m_iterable->deduce_owned_adaptor();
    }

public:
    typedef typename Traits traits;
    typedef detail::const_iterable_traits<traits> const_traits;

    TEMPLATE(typename Iterable)
    REQUIRES(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits))
    type_erasure_iterable_impl(const iterable<Iterable>& i_iterable);
    TEMPLATE(typename Iterable)
    REQUIRES(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits))
    type_erasure_iterable_impl(iterable<Iterable>&& i_iterable);
    TEMPLATE(typename Iterable, typename Allocator)
    REQUIRES(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits),IS_ALLOCATOR(Allocator))
    type_erasure_iterable_impl(const iterable<Iterable>& i_iterable,const Allocator& i_allocator);
    TEMPLATE(typename Iterable, typename Allocator)
    REQUIRES(ARE_TRAITS_COMPATIBLE(typename Iterable::traits,Traits),IS_ALLOCATOR(Allocator))
    type_erasure_iterable_impl(iterable<Iterable>&& i_iterable,const Allocator& i_allocator);
    type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<Traits>>& i_iterable);
    type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<Traits>>&& i_iterable);
    TEMPLATE(typename TTraits)
    REQUIRES(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
    type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<TTraits>>& i_iterable);
    TEMPLATE(typename TTraits)
    REQUIRES(ARE_TRAITS_COMPATIBLE(TTraits,Traits))
    type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable);
    TEMPLATE(typename TTraits,typename Allocator)
    REQUIRES(ARE_TRAITS_COMPATIBLE(TTraits,Traits),IS_ALLOCATOR(Allocator))
    type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<TTraits>>& i_iterable,const Allocator& i_allocator);
    TEMPLATE(typename TTraits,typename Allocator)
    REQUIRES(ARE_TRAITS_COMPATIBLE(TTraits,Traits),IS_ALLOCATOR(Allocator))
    type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable,const Allocator& i_allocator);
    type_erasure_iterable_impl(type_erasure_iterable_impl&&) = default;

    template<typename Action>
    void iterate_impl(const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const Action& i_initialAction) const;
    distributed_reference_wrapper<iterable_interface<Traits>> get_iterable() const;

private:
    distributed_reference_wrapper<iterable_interface<Traits>> m_iterable;
};
template<typename Iterable>
type_erasure_iterable_impl(Iterable&&) -> type_erasure_iterable_impl<typename Iterable::traits>;

}
}

#include "ddk_type_erasure_iterable_impl.inl"