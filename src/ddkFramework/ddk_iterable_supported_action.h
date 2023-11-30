#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_type_erasured_iterable_adaptor.h"
#include "ddk_function.h"

namespace ddk
{

template<typename Traits>
struct supported_action : action_base
{
    friend inline supported_action inverse(const supported_action& other)
    {
        return other;
    }

public:
    typedef typename Traits::tags_t tags_t;

    TEMPLATE(typename Action)
    REQUIRES(ACTION_TAGS_SUPPORTED(Traits,typename Action::tags_t))
    supported_action(const Action& i_action);
    supported_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;

private:
    function<bool(type_erasure_iterable_adaptor<Traits>)> m_action;
};

}

#include "ddk_iterable_supported_action.inl"