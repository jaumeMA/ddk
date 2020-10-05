#pragma once

#include "ddk_iterable_state.h"
#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Iterable, typename Action>
struct action_visitor;

template<typename Iterable>
using iterable_iterator = typename mpl::static_if<std::is_const<Iterable>::value,typename Iterable::const_iterator,typename Iterable::iterator>::type;

template<typename Iterable>
struct action_visitor<Iterable,input_action> : public static_visitor<iterable_iterator<Iterable>>
{
public:
    action_visitor(Iterable& i_iterable, iterable_iterator<Iterable> i_currIterator);

    iterable_iterator<Iterable> visit(const stop_action&);
    iterable_iterator<Iterable> visit(const erase_action&);
    iterable_iterator<Iterable> visit(const add_action&);
    iterable_iterator<Iterable> visit(const go_forward_action&);

protected:
    Iterable& m_iterable;
    iterable_iterator<Iterable> m_currIterator;
};

template<typename Iterable>
struct action_visitor<Iterable,bidirectional_action> : public action_visitor<Iterable,input_action>
{
public:
    using action_visitor<Iterable,input_action>::action_visitor;
    using action_visitor<Iterable,input_action>::visit;

    iterable_iterator<Iterable> visit(const go_backward_action&);
};

template<typename Iterable>
struct action_visitor<Iterable,random_access_action> : public action_visitor<Iterable,bidirectional_action>
{
public:
    using action_visitor<Iterable,bidirectional_action>::action_visitor;
    using action_visitor<Iterable,bidirectional_action>::visit;

    iterable_iterator<Iterable> visit(const shift_action&);
};

}

template<typename Iterable, typename Action, typename Reference>
inline Reference visit_iterator(Iterable& i_iterable, const function<Action(Reference)>& i_sink, const iter::iterable_state& i_initState = iter::iterable_state());

}
}

#include "ddk_iterable_visitor.inl"
