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

template<typename Iterable, typename Adaptor>
struct action_visitor_base : public static_visitor<typename Adaptor::reference>
{
public:
	template<typename Action>
	action_visitor_base(Iterable& i_iterable, Action&& i_initialAction);

protected:
	Adaptor m_adaptor;
};

template<typename Iterable, typename Action, typename AAction = Action>
struct action_visitor;

template<typename Iterable, typename Adaptor>
struct action_visitor<Iterable,input_action,Adaptor> : public action_visitor_base<Iterable,Adaptor>
{
public:
    using action_visitor_base<Iterable,Adaptor>::action_visitor_base;
	using action_visitor_base<Iterable,Adaptor>::visit;
	typedef typename Adaptor::reference reference;
	typedef typename Adaptor::const_reference const_reference;

	inline reference visit(const stop_action&);
	inline reference visit(const erase_action&);
	inline reference visit(const add_action&);
	inline reference visit(const go_forward_action&);
};

template<typename Iterable, typename Adaptor>
struct action_visitor<Iterable,bidirectional_action,Adaptor> : public action_visitor<Iterable,input_action,Adaptor>
{
public:
    using action_visitor<Iterable,input_action,Adaptor>::action_visitor;
    using action_visitor<Iterable,input_action,Adaptor>::visit;
	typedef typename Adaptor::reference reference;
	typedef typename Adaptor::const_reference const_reference;

	inline reference visit(const go_backward_action&);
};

template<typename Iterable, typename Adaptor>
struct action_visitor<Iterable,random_access_action,Adaptor> : public action_visitor<Iterable,bidirectional_action,Adaptor>
{
public:
    using action_visitor<Iterable,bidirectional_action,Adaptor>::action_visitor;
    using action_visitor<Iterable,bidirectional_action,Adaptor>::visit;
	typedef typename Adaptor::reference reference;
	typedef typename Adaptor::const_reference const_reference;

	inline reference visit(const shift_action&);
};

}

template<typename Iterable, typename Action, typename Reference>
inline Reference visit_iterator(Iterable& i_iterable, const function<Action(Reference)>& i_sink, const iter::shift_action& i_initialAction = iter::go_to_place);

}
}

#include "ddk_iterable_visitor.inl"
