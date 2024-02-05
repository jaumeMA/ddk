#pragma once

#include "ddk_optional.h"
#include "ddk_iterable_action_tags.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

struct action_base
{
public:
    typedef long long difference_type;

    action_base() = default;
    action_base(const action_base&) = default;
    action_base(bool i_valid);

    explicit operator bool() const;

private:
    bool m_valid = true;
};

struct no_action : action_base
{
public:
    typedef mpl::empty_type_pack tags_t;

    no_action() = default;
    no_action(bool i_valid);

    template<typename Adaptor>
    no_action apply(Adaptor&& i_adaptor) const;
};

struct remove_action : action_base
{
public:
    typedef mpl::type_pack<backward_action_tag> tags_t;

    remove_action() = default;
    remove_action(bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline no_action apply(Adaptor&& i_adaptor) const;
};

template<typename T>
struct add_action : action_base
{
public:
    typedef mpl::type_pack<add_action_tag<T>> tags_t;

    add_action(T i_value);
    add_action(T i_value, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline no_action apply(Adaptor && i_adaptor) const;

private:
    mutable T m_value;
};
template<typename T>
add_action(T) -> add_action<T>;

struct forward_action : action_base
{
public:
    typedef mpl::type_pack<forward_action_tag> tags_t;

    forward_action() = default;
    forward_action(bool i_value);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline forward_action apply(Adaptor && i_adaptor) const;
};

struct backward_action : action_base
{
public:
    typedef mpl::type_pack<backward_action_tag> tags_t;

    backward_action() = default;
    backward_action(bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline backward_action apply(Adaptor&& i_adaptor) const;
};

struct go_to_begin_action : forward_action
{
public:
    typedef mpl::type_pack<begin_action_tag,forward_action_tag> tags_t;

    go_to_begin_action() = default;
    go_to_begin_action(bool i_valid);
    go_to_begin_action(const forward_action&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline forward_action apply(Adaptor&& i_adaptor) const;
};

struct go_to_end_action : backward_action
{
public:
    typedef mpl::type_pack<last_action_tag,backward_action_tag> tags_t;

    go_to_end_action() = default;
    go_to_end_action(bool i_valud);
    go_to_end_action(const backward_action&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline backward_action apply(Adaptor&& i_adaptor) const;
};

struct displacement_action : action_base
{
    friend inline displacement_action stop_action(const displacement_action& i_action)
    {
        return { i_action.m_shift,false };
    }

public:
    typedef mpl::type_pack<displace_action_tag> tags_t;

    displacement_action(difference_type i_targetShift);
    displacement_action(difference_type i_targetShift, bool i_valid);

    difference_type shift() const;
    void set_shift(difference_type i_shift);
    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline no_action apply(Adaptor&& i_adaptor) const;

private:
    difference_type m_shift;
};

struct bidirectional_action : action_base
{
public:
    typedef mpl::type_pack<forward_action_tag,backward_action_tag> tags_t;

    bidirectional_action(bool i_foward);
    bidirectional_action(bool i_foward, bool i_valid);
    bidirectional_action(const displacement_action& i_action, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline bidirectional_action apply(Adaptor&& i_adaptor) const;

private:
    const bool m_forward;
};

template<typename Sink>
struct sink_action : action_base
{
    template<typename>
    friend struct sink_action;

public:
    typedef mpl::type_pack<sink_action_tag<Sink>> tags_t;

    sink_action(const Sink& i_sink);
    sink_action(Sink&& i_sink);
    sink_action(const Sink& i_sink, bool i_valid);
    sink_action(Sink&& i_sink, bool i_valid);
    TEMPLATE(typename SSink)
    REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
    sink_action(const sink_action<SSink>& i_action);
    TEMPLATE(typename SSink)
    REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
    sink_action(sink_action<SSink>&& i_action);
    sink_action(const sink_action&) = default;
    sink_action(sink_action&&) = default;

    sink_action& operator=(const sink_action&) = default;
    sink_action& operator=(sink_action&&) = default;

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor&& i_adaptor) const;

private:
    Sink m_sink;
};

template<typename Action, typename Sink>
struct action_sink : action_base
{
public:
    typedef typename Action::tags_t::template add_unique<sink_action_tag<Sink>>::type tags_t;

    action_sink(const Action& i_action,const Sink& i_sink);
    action_sink(Action&& i_action,Sink&& i_sink);
    action_sink(const Action& i_action, const Sink& i_sink,bool i_valid);
    action_sink(Action&& i_action,Sink&& i_sink,bool i_valid);
    action_sink(const action_sink&) = default;
    action_sink(action_sink&&) = default;

    action_sink& operator=(const action_sink&) = default;
    action_sink& operator=(action_sink&&) = default;

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    inline auto apply(Adaptor && i_adaptor) const;

private:
    Action m_action;
    Sink m_sink;
};
template<typename Action, typename Sink>
action_sink(const Action& i_action,const Sink& i_sink,bool i_valid) -> action_sink<Action,Sink>;
template<typename Action,typename Sink>
action_sink(Action&& i_action, Sink&& i_sink,bool i_valid) -> action_sink<Action,Sink>;

}

#include "ddk_iterable_action_ops.h"
#include "ddk_iterable_action_defs.inl"