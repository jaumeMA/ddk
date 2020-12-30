#pragma once

#include "ddk_function.h"
#include "ddk_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_inherited_value.h"
#include "ddk_any_value.h"

namespace ddk
{
namespace detail
{

template<typename,typename,typename>
class dynamic_multi_visitor_base;
	
template<typename MultiVisitor,typename Visitor,typename Type>
class dynamic_multi_visitor_base : public typed_dynamic_visitor<Type>
{
public:
	typedef Type type;

	dynamic_multi_visitor_base() = default;

	void visit(const Type& i_value) override;
	void visit(const Type& i_value) const override;

	static const bool __register_type_visitor;
};

}

template<typename,typename,typename,typename ...>
class dynamic_multi_visitor;

template<typename Visitor,typename ... Types,typename ... ResolvedTypes, typename Value, typename ... Values>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>
	: public dynamic_visitor<typename Value::type_interface>
	, public detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>,Visitor,Types> ...
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(Visitor& i_visitor,const inherited_value<Value>& i_value,const inherited_value<Values>& ... i_pendingValues);

	function<return_type(ResolvedTypes...)> visit() const;
	template<typename T>
	inline void typed_visit(T&& i_resolvedValue) const;

private:
	template<size_t ... IndexsResolved,size_t ... IndexsToResolve, typename T>
	inline void typed_visit(const mpl::sequence<IndexsResolved...>&,const mpl::sequence<IndexsToResolve...>&,T&&) const;

	Visitor& m_visitor;
	inherited_value<Value> m_value;
	tuple<inherited_value<Values> ...> m_pendingValues;
	mutable function<return_type(ResolvedTypes...)> m_resolvedFunction;
};

template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(Visitor& i_visitor);
	function<return_type(ResolvedTypes...)> visit() const;

private:
	Visitor& m_visitor;
};

TEMPLATE(typename Visitor,typename ... Values)
REQUIRES(IS_CALLABLE(Visitor),IS_BASE_OF(typename Visitor::type_interface,Values)...)
inline typename Visitor::return_type visit(const Visitor& i_callable,const inherited_value<Values>& ... i_values);

TEMPLATE(typename Visitor,typename ... Values)
REQUIRES(IS_CALLABLE(Visitor),IS_BASE_OF(typename Visitor::type_interface,Values)...)
inline typename Visitor::return_type visit(Visitor& i_callable,const inherited_value<Values>& ... i_values);

}

#include "ddk_dynamic_multivisitor.inl"