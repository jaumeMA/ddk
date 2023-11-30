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
class iterable : public iterable_base<typename Iterable::traits>
{
    template<typename TTraits>
    friend class iterable;

    friend inline auto deduce_adaptor(const iterable& i_iterable)
    {
        return deduce_adaptor(i_iterable.m_iterableImpl);
    }

    typedef iterable_base<typename Iterable::traits> base_t;
    using typename base_t::action;

public:
    using typename base_t::traits;
    using typename base_t::value_type;
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::terse_endpoint;
    using typename base_t::const_terse_endpoint;
    typedef Iterable iterable_t;

    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,Args...))
    iterable(Args&& ... i_args);
    iterable(const iterable&) = delete;
    iterable(iterable&&) = default;

    TEMPLATE(typename Function, typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference),ACTION_TAGS_SUPPORTED(traits,typename Action::tags_t))
    iterable_result iterate_impl(Function&& i_try, const Action& i_initialAction);
    TEMPLATE(typename Function, typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,const_reference),ACTION_TAGS_SUPPORTED(traits,typename Action::tags_t))
    iterable_result iterate_impl(Function&& i_try, const Action& i_initialAction) const;
    bool inline operator==(const std::nullptr_t&) const;
    bool inline operator!=(const std::nullptr_t&) const;
    inline const Iterable& get() const;
    inline Iterable&& extract() &&;

private:
    iterable_result iterate(terse_endpoint i_try,const action& i_initialAction) override;
    iterable_result iterate(const_terse_endpoint i_try,const action& i_initialAction) const override;

    iterable_t m_iterableImpl;
};
template<typename Iterable>
iterable(Iterable&&)->iterable<Iterable>;

}
}

#include "ddk_agnostic_iterable.h"
#include "ddk_iterable_resolver.h"
#include "ddk_iterable_action_resolver.h"
#include "ddk_iterable_order_resolver.h"
#include "ddk_view_utils.h"
#include "ddk_iterable_utils.h"
#include "ddk_builtin_iterables.h"
#include "ddk_iterable_action_defs_ext.h"

#include "ddk_iterable.inl"
#include "ddk_iterable_order_resolver.inl"
#include "ddk_view_utils.inl"
#include "ddk_iterable_utils.inl"
#include "ddk_builtin_iterables.inl"