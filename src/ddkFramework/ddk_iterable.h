#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_interface.h"
#include "ddk_iterable_traits.h"
#include "ddk_iterable_type.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
class iterable
{
    template<typename TTraits>
    friend class iterable;

    friend inline auto deduce_adaptor(iterable& i_iterable)
    {
        return deduce_adaptor(i_iterable.m_iterableImpl);
    }

public:
    typedef typename Iterable::traits traits;
    typedef detail::const_iterable_traits<traits> const_traits;
    typedef Iterable iterable_t;

    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,Args...))
    iterable(Args&& ... i_args);
    iterable(const iterable&) = delete;
    iterable(iterable&&) = default;

    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(traits,Action))
    void iterate_impl(Action&& i_initialAction);
    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(const_traits,Action))
    void iterate_impl(Action&& i_initialAction) const;
    inline Iterable& get();
    inline const Iterable& get() const;
    inline Iterable&& extract() &&;

private:
    iterable_t m_iterableImpl;
};
template<typename Iterable>
iterable(const Iterable&)->iterable<Iterable>;
template<typename Iterable>
iterable(Iterable&&)->iterable<Iterable>;

template<typename Iterable>
typename iterable<Iterable>::traits deduce_iterable_traits(const iterable<Iterable>&);

}
}

#include "ddk_iterable_resolver.h"
#include "ddk_iterable_action_resolver.h"
#include "ddk_iterable_order_resolver.h"
#include "ddk_view_utils.h"
#include "ddk_iterable_utils.h"
#include "ddk_agnostic_iterable.h"
#include "ddk_builtin_iterables.h"
#include "ddk_iterable_action_defs_ext.h"

#include "ddk_iterable.inl"
#include "ddk_iterable_order_resolver.inl"
#include "ddk_view_utils.inl"
#include "ddk_iterable_utils.inl"
#include "ddk_builtin_iterables.inl"