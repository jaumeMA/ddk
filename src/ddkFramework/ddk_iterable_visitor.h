#pragma once

#include "ddk_iterable_state.h"
#include "ddk_iterable_action.h"
#include "ddk_scoped_enum.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Iterable>
using iterable_iterator = typename mpl::static_if<std::is_const<Iterable>::value, typename Iterable::const_iterator, typename Iterable::iterator>::type;

template<typename Iterable, typename Action>
struct action_visitor_base : public static_visitor<iterable_iterator<Iterable>>
{
public:
	action_visitor_base(Iterable& i_iterable, iterable_iterator<Iterable> i_currIterator, const iter::iterable_state& i_iterableState);

protected:
	virtual iterable_iterator<Iterable> reapply_action(const Action&) = 0;

	Iterable& m_iterable;
	iterable_iterator<Iterable> m_currIterator;
	iter::iterable_state m_iterableState;
	Action m_lastAction;
};

template<typename Iterable, typename Action, typename AAction = Action>
struct action_visitor;

template<typename Iterable, typename AAction>
struct action_visitor<Iterable,input_action,AAction> : public action_visitor_base<Iterable, AAction>
{
public:
    using action_visitor_base<Iterable,AAction>::action_visitor_base;
	using action_visitor_base<Iterable,AAction>::visit;

	iterable_iterator<Iterable> visit(const filter_action&);
	iterable_iterator<Iterable> visit(const stop_action&);
	iterable_iterator<Iterable> visit(const erase_action&);
	iterable_iterator<Iterable> visit(const add_action&);
	iterable_iterator<Iterable> visit(const go_forward_action&);

private:
	iterable_iterator<Iterable> reapply_action(const input_action&);
};

template<typename Iterable, typename AAction>
struct action_visitor<Iterable,bidirectional_action,AAction> : public action_visitor<Iterable,input_action,AAction>
{
public:
    using action_visitor<Iterable,input_action,AAction>::action_visitor;
    using action_visitor<Iterable,input_action,AAction>::visit;

    iterable_iterator<Iterable> visit(const go_backward_action&);

private:
	iterable_iterator<Iterable> reapply_action(const bidirectional_action&);
};

template<typename Iterable, typename AAction>
struct action_visitor<Iterable,random_access_action,AAction> : public action_visitor<Iterable,bidirectional_action,AAction>
{
public:
    using action_visitor<Iterable,bidirectional_action,AAction>::action_visitor;
    using action_visitor<Iterable,bidirectional_action,AAction>::visit;

    iterable_iterator<Iterable> visit(const shift_action&);

private:
	iterable_iterator<Iterable> reapply_action(const random_access_action&);
};

}

template<typename Iterable, typename Action, typename Reference>
inline Reference visit_iterator(Iterable& i_iterable, const function<Action(Reference)>& i_sink, const iter::iterable_state& i_initState = iter::iterable_state());

}
}

#include "ddk_iterable_visitor.inl"
