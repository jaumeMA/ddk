#pragma once

#include "ddk_iterable_action_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Action,typename AAction>
struct and_action : action_base
{
    friend inline and_action stop_action(const and_action& i_action)
    {
        return { i_action.m_currentAction,i_action.m_pendingAction,false };
    }

public:
    typedef typename mpl::merge_type_packs<typename Action::tags_t,typename AAction::tags_t>::type tags_t;

    and_action(const Action& i_lhs,const AAction& i_rhs, bool i_valid = true);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor) const;

private:
    Action m_currentAction;
    AAction m_pendingAction;
};

template<typename Action>
struct and_action<Action,no_action> : action_base
{
    friend inline and_action stop_action(const and_action& i_action)
    {
        return { i_action.m_currentAction,false };
    }

public:
    typedef typename Action::tags_t tags_t;

    and_action(const Action& i_lhs, const no_action&, bool i_valid = true);
    and_action(const Action& i_lhs, bool i_valid = true);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor) const;

private:
    Action m_currentAction;
};
template<typename Action>
struct and_action<no_action,Action> : action_base
{
    friend inline and_action stop_action(const and_action& i_action)
    {
        return { i_action.m_currentAction,false };
    }

public:
    typedef typename Action::tags_t tags_t;

    and_action(const no_action&, const Action& i_lhs, bool i_valid = true);
    and_action(const Action& i_lhs, bool i_valid = true);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor) const;

private:
    Action m_currentAction;
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

}

#include "ddk_iterable_action_ops.inl"