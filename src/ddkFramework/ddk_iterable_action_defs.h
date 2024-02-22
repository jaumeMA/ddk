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

    constexpr action_base() = default;
    constexpr action_base(const action_base&) = default;
    constexpr action_base(bool i_valid);

    constexpr explicit operator bool() const;
    constexpr action_base& operator=(const action_base& i_action);

private:
    bool m_valid = true;
};

struct no_action : action_base
{
public:
    typedef mpl::empty_type_pack tags_t;

    constexpr no_action() = default;
    constexpr no_action(bool i_valid);

    template<typename Adaptor>
    constexpr no_action apply(Adaptor&& i_adaptor) const;
};

struct remove_action : action_base
{
public:
    typedef mpl::type_pack<backward_action_tag> tags_t;

    constexpr remove_action() = default;
    constexpr remove_action(bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline no_action apply(Adaptor&& i_adaptor) const;
};

template<typename T>
struct add_action : action_base
{
public:
    typedef mpl::type_pack<add_action_tag<T>> tags_t;

    constexpr add_action(T i_value);
    constexpr add_action(T i_value, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline no_action apply(Adaptor && i_adaptor) const;

private:
    mutable T m_value;
};
template<typename T>
add_action(T) -> add_action<T>;

struct forward_action : action_base
{
public:
    typedef mpl::type_pack<forward_action_tag> tags_t;

    constexpr forward_action() = default;
    constexpr forward_action(bool i_value);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline forward_action apply(Adaptor && i_adaptor) const;
    constexpr forward_action& operator=(const forward_action&);
};

struct backward_action : action_base
{
public:
    typedef mpl::type_pack<backward_action_tag> tags_t;

    constexpr backward_action() = default;
    constexpr backward_action(bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline backward_action apply(Adaptor&& i_adaptor) const;
};

struct go_to_begin_action : forward_action
{
public:
    typedef mpl::type_pack<begin_action_tag,forward_action_tag> tags_t;

    constexpr go_to_begin_action() = default;
    constexpr go_to_begin_action(bool i_valid);
    constexpr go_to_begin_action(const forward_action&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline forward_action apply(Adaptor&& i_adaptor) const;
    constexpr go_to_begin_action& operator=(const go_to_begin_action&);
};

struct go_to_end_action : backward_action
{
public:
    typedef mpl::type_pack<end_action_tag,backward_action_tag> tags_t;

    constexpr go_to_end_action() = default;
    constexpr go_to_end_action(bool i_valud);
    constexpr go_to_end_action(const backward_action&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline backward_action apply(Adaptor&& i_adaptor) const;
};

struct displacement_action : action_base
{
    friend inline displacement_action stop_action(const displacement_action& i_action)
    {
        return { i_action.m_shift,false };
    }

public:
    typedef mpl::type_pack<displace_action_tag> tags_t;

    constexpr displacement_action(difference_type i_targetShift);
    constexpr displacement_action(difference_type i_targetShift, bool i_valid);

    constexpr difference_type shift() const;
    constexpr void set_shift(difference_type i_shift);
    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline no_action apply(Adaptor&& i_adaptor) const;

private:
    difference_type m_shift;
};

struct bidirectional_action : action_base
{
public:
    typedef mpl::type_pack<forward_action_tag,backward_action_tag> tags_t;

    constexpr bidirectional_action(bool i_foward);
    constexpr bidirectional_action(bool i_foward, bool i_valid);
    constexpr bidirectional_action(const displacement_action& i_action, bool i_valid);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline bidirectional_action apply(Adaptor&& i_adaptor) const;

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

    constexpr sink_action(const Sink& i_sink);
    constexpr sink_action(Sink&& i_sink);
    constexpr sink_action(const Sink& i_sink, bool i_valid);
    constexpr sink_action(Sink&& i_sink, bool i_valid);
    TEMPLATE(typename SSink)
    REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
    constexpr sink_action(const sink_action<SSink>& i_action);
    TEMPLATE(typename SSink)
    REQUIRES(IS_CONSTRUCTIBLE(Sink,SSink))
    constexpr sink_action(sink_action<SSink>&& i_action);
    constexpr sink_action(const sink_action&) = default;
    constexpr sink_action(sink_action&&) = default;

    constexpr sink_action& operator=(const sink_action& i_action);
    constexpr sink_action& operator=(sink_action&& i_action);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline auto apply(Adaptor&& i_adaptor) const;

private:
    Sink m_sink;
};

template<typename Action, typename Sink>
struct action_sink : action_base
{
public:
    typedef typename Action::tags_t::template add_unique<sink_action_tag<Sink>>::type tags_t;

    constexpr action_sink(const Action& i_action,const Sink& i_sink);
    constexpr action_sink(Action&& i_action,Sink&& i_sink);
    constexpr action_sink(const Action& i_action, const Sink& i_sink,bool i_valid);
    constexpr action_sink(Action&& i_action,Sink&& i_sink,bool i_valid);
    constexpr action_sink(const action_sink&) = default;
    constexpr action_sink(action_sink&&) = default;

    constexpr action_sink& operator=(const action_sink&);
    constexpr action_sink& operator=(action_sink&&);

    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline auto apply(Adaptor && i_adaptor) const;

private:
    mutable Action m_action;
    mutable Sink m_sink;
};
template<typename Action, typename Sink>
action_sink(const Action& i_action,const Sink& i_sink,bool i_valid) -> action_sink<Action,Sink>;
template<typename Action,typename Sink>
action_sink(Action&& i_action, Sink&& i_sink,bool i_valid) -> action_sink<Action,Sink>;

struct size_action : action_base
{
public:
    typedef mpl::type_pack<size_action_tag> tags_t;

    static const size_t nsize = -1;

    constexpr size_action() = default;
    constexpr size_action(bool i_valid);

    constexpr size_t operator()() const;
    constexpr operator bool() const;
    TEMPLATE(typename Adaptor)
    REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
    constexpr inline no_action apply(Adaptor&& i_adaptor) const;

private:
    mutable size_t m_size = nsize;
};

}

#include "ddk_iterable_action_ops.h"
#include "ddk_iterable_action_defs.inl"