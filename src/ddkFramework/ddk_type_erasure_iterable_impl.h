#pragma once

#include "ddk_iterable_interface.h"
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
class type_erasure_iterable_impl : public iterable_impl_interface<typename Traits>
{
    friend inline auto lend(const type_erasure_iterable_impl& i_iterable)
    {
        return lend_type_erasure_iterable_impl{lend(i_iterable.m_iterable)};
    }

public:
    typedef typename Traits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;
    typedef supported_action<traits> action;
    typedef mpl::terse_callable<function<action(reference)>> terse_endpoint;
    typedef mpl::terse_callable<function<action(const_reference)>> const_terse_endpoint;

    template<typename Iterable>
    type_erasure_iterable_impl(const iterable<Iterable>& i_iterable);
    template<typename Iterable>
    type_erasure_iterable_impl(iterable<Iterable>&& i_iterable);
    template<typename Iterable,typename Allocator>
    type_erasure_iterable_impl(const iterable<Iterable>& i_iterable,const Allocator& i_allocator);
    template<typename Iterable, typename Allocator>
    type_erasure_iterable_impl(iterable<Iterable>&& i_iterable,const Allocator& i_allocator);
    type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<Traits>>& i_iterable);
    type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<Traits>>&& i_iterable);
    template<typename TTraits>
    type_erasure_iterable_impl(const iterable<type_erasure_iterable_impl<TTraits>>& i_iterable);
    template<typename TTraits>
    type_erasure_iterable_impl(iterable<type_erasure_iterable_impl<TTraits>>&& i_iterable);
    type_erasure_iterable_impl(type_erasure_iterable_impl&&) = default;

    template<typename Action>
    void iterate_impl(terse_endpoint i_try,const Action& i_initialAction);
    template<typename Action>
    void iterate_impl(const_terse_endpoint i_try,const Action& i_initialAction) const;

private:
    distributed_reference_wrapper<iterable_base<Traits>> m_iterable;
};
template<typename Iterable>
type_erasure_iterable_impl(Iterable&&)->type_erasure_iterable_impl<typename Iterable::traits>;

}
}

#include "ddk_type_erasure_iterable_impl.inl"