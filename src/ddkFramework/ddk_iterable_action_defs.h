#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

struct stop_action
{
    typedef long long difference_type;

    stop_action() = default;
    difference_type shift() const;

private:
    static const difference_type npos = -1;
};

struct action_base
{
public:
    typedef long long difference_type;

    action_base(const action_base&) = default;
    action_base(bool i_valid = true);

    operator bool() const;

private:
    bool m_valid = true;
};

struct no_action : action_base
{
    typedef mpl::type_pack<forward_action_tag> tags_t;

    no_action() = default;
    no_action(const stop_action&);
    
    template<typename Adaptor, typename Sink>
    no_action apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
};

struct remove_action : action_base
{
    typedef mpl::type_pack<backward_action_tag> tags_t;

    remove_action() = default;
    remove_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
};

template<typename T>
struct add_action : action_base
{
public:
    typedef mpl::type_pack<add_action_tag<T>> tags_t;

    add_action() = default;
    add_action(T i_value);
    add_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor && i_adaptor,Sink && i_sink) const;

private:
    mutable T m_value;
};
template<typename T>
add_action(T) -> add_action<T>;

struct forward_action : action_base
{
    typedef mpl::type_pack<forward_action_tag> tags_t;

    forward_action() = default;
    forward_action(const stop_action&);

    TEMPLATE(typename Adaptor, typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor && i_adaptor,Sink && i_sink) const;
};

struct backward_action : action_base
{
    typedef mpl::type_pack<backward_action_tag> tags_t;

    backward_action() = default;
    backward_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
};

struct go_to_begin_action : forward_action
{
public:
    typedef mpl::type_pack<begin_action_tag,forward_action_tag> tags_t;

    go_to_begin_action() = default;
    go_to_begin_action(const forward_action&);
    go_to_begin_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
};

struct go_to_end_action : backward_action
{
public:
    typedef mpl::type_pack<last_action_tag,backward_action_tag> tags_t;

    go_to_end_action() = default;
    go_to_end_action(const backward_action&);
    go_to_end_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
};

struct displacement_action : action_base
{
public:
    typedef mpl::type_pack<displace_action_tag> tags_t;

    displacement_action(difference_type i_targetShift);
    displacement_action(const stop_action&);

    difference_type shift() const;
    void set_shift(difference_type i_shift);
    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;

private:
    difference_type m_shift;
};

struct bidirectional_action : action_base
{
public:
    typedef mpl::type_pack<forward_action_tag,backward_action_tag> tags_t;

    bidirectional_action(bool i_foward);
    bidirectional_action(const displacement_action& i_action);
    bidirectional_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;

private:
    const bool m_forward;
};

template<typename Action>
struct step_by_step_action : action_base
{
    template<typename>
    friend struct step_by_step_action;
    friend inline auto inverse(const step_by_step_action& i_action)
    {
        return step_by_step_action<decltype(inverse(std::declval<Action>()))>{ inverse(i_action.m_action) };
    }

public:
    typedef typename Action::tags_t tags_t;

    TEMPLATE(typename AAction)
    REQUIRES(IS_CONSTRUCTIBLE(Action,AAction))
    step_by_step_action(AAction&& i_action);
    TEMPLATE(typename AAction)
    REQUIRES(IS_CONSTRUCTIBLE(Action,AAction))
    step_by_step_action(const step_by_step_action<AAction>& other);
    step_by_step_action(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;
    step_by_step_action& operator--();
    bool ready() const;

private:
    Action m_action;
    size_t m_steps = 0;
};
template<typename Action>
step_by_step_action(const Action&) -> step_by_step_action<typename mpl::which_type<mpl::is_same_type<Action,displacement_action>::value,bidirectional_action,Action>::type>;

template<typename Action, typename AAction>
struct action_pair : Action
{
    static const size_t s_num_pending_actions = mpl::num_types<Actions...>;

public:
    action_pair(const Action& i_lhs, const AAction& i_rhs);
    action_pair(const stop_action&);

    TEMPLATE(typename Adaptor,typename Sink)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;

private:
    AAction m_pendingAction;
};
template<typename Action,typename AAction>
action_pair(const Action&,const AAction&) -> action_pair<Action,AAction>;

}

#include "ddk_iterable_action_defs.inl"
#include "ddk_iterable_action_ops.h"