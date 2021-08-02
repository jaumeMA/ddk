#pragma once

#include "ddk_function.h"
#include "ddk_concepts.h"
#include "ddk_inherited_value_concepts.h"
#include "ddk_any_value.h"

namespace ddk
{
namespace detail
{

template<typename MultiVisitor,typename Visitor,typename Type>
class dynamic_multi_visitor_base : public typed_dynamic_visitor<Type>
{
public:
	typedef Type type;
    typedef typename Visitor::type_interface type_interface;

	dynamic_multi_visitor_base() = default;

	void visit(Type& i_value) override;
	void visit(const Type& i_value) override;

	static const bool __register_type_visitor;
};

}

template<typename,typename,typename,typename ...>
class dynamic_multi_visitor;

template<typename Visitor,typename ... Types,typename ... ResolvedTypes, typename Value, typename ... Values>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>
	: public dynamic_visitor<typename Visitor::type_interface>
	, public detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>,Visitor,Types> ...
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(Visitor& i_visitor,const Value& i_value,const Values& ... i_pendingValues);

	function<return_type(ResolvedTypes...)> visit();
	template<typename T>
	inline void typed_visit(T&& i_resolvedValue);

private:
	template<size_t ... IndexsResolved,size_t ... IndexsToResolve, typename T>
	inline void typed_visit(const mpl::sequence<IndexsResolved...>&,const mpl::sequence<IndexsToResolve...>&,T&&);

	Visitor& m_visitor;
	Value m_value;
	tuple<Values ...> m_pendingValues;
	mutable function<return_type(ResolvedTypes...)> m_resolvedFunction;
};

template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(Visitor& i_visitor);
	function<return_type(ResolvedTypes...)> visit();

private:
	Visitor& m_visitor;
};

TEMPLATE(typename Callable,typename ... Values)
REQUIRES(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
inline auto visit(Callable&& i_callable,const Values& ... i_values);

TEMPLATE(typename Return,typename Callable,typename ... Values)
REQUIRES(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
inline auto visit(Callable&& i_callable,const Values& ... i_values);

TEMPLATE(typename Callable,typename ... Values)
REQUIRES(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
inline auto visit(const Values& ... i_values);

TEMPLATE(typename Return,typename Callable,typename ... Values)
REQUIRES(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
inline auto visit(const Values& ... i_values);

}

#include "ddk_dynamic_multivisitor.inl"
