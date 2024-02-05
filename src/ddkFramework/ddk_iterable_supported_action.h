#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_type_erasured_iterable_adaptor.h"
#include "ddk_function.h"

namespace ddk
{

template<typename Traits>
struct supported_action : action_base
{
public:
    typedef typename Traits::tags_t tags_t;
    typedef typename Traits::const_tags_t const_tags_t;
    typedef supported_action<Traits> action_t;

    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(Traits,Action))
    supported_action(const Action& i_action, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor& i_adaptor) const;
    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(const Adaptor,const_tags_t))
    inline auto apply(const Adaptor& i_adaptor) const;

private:
    function<action_t(iterable_adaptor<detail::type_erasure_iterable_impl<Traits>>)> m_action;
};

template<typename Traits>
struct const_supported_action : action_base
{
public:
    typedef Traits traits;
    typedef detail::const_iterable_traits<traits> const_traits;
    typedef typename traits::const_tags_t tags_t;
    typedef typename traits::const_tags_t const_tags_t;
    typedef const_supported_action<Traits> action_t;

    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(const_traits,Action))
    const_supported_action(const Action& i_action, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(const Adaptor,const_tags_t))
    inline auto apply(const Adaptor& i_adaptor) const;

private:
    function<action_t(iterable_adaptor<const detail::type_erasure_iterable_impl<const_traits>>)> m_action;
};

}

#include "ddk_iterable_supported_action.inl"