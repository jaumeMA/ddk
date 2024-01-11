#pragma once

#include "ddk_iterable_action_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Action,typename AAction>
struct and_action : Action
{
public:
    and_action(const Action& i_lhs,const AAction& i_rhs);
    and_action(const stop_action&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor) const;

private:
    AAction m_pendingAction;
};
template<typename Action,typename AAction>
and_action(const Action&,const AAction&) ->and_action<Action,AAction>;

TEMPLATE(typename Action,typename AAction)
REQUIRES(IS_ITERABLE_ACTION(Action),IS_ITERABLE_ACTION(AAction))
inline auto operator&&(Action&& i_lhs, AAction&& i_rhs);

TEMPLATE(typename Action)
REQUIRES(IS_ITERABLE_ACTION(Action))
inline auto operator&&(Action&& i_lhs,const no_action&);

TEMPLATE(typename Action)
REQUIRES(IS_ITERABLE_ACTION(Action))
inline auto operator&&(const no_action&,Action&& i_rhs);

TEMPLATE(typename Action)
REQUIRES(IS_ITERABLE_ACTION(Action))
inline auto operator&&(Action&& i_lhs,const stop_action&);

TEMPLATE(typename Action)
REQUIRES(IS_ITERABLE_ACTION(Action))
inline auto operator&&(const stop_action&,Action&& i_rhs);

}

#include "ddk_iterable_action_ops.inl"