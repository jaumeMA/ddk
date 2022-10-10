#pragma once

#include "ddk_function.h"
#include "ddk_concepts.h"
#include "ddk_inherited_value_concepts.h"

namespace ddk
{
namespace detail
{

template<typename,typename...>
struct specialized_visitor;

template<typename Visitor,typename T,typename ... TT>
struct specialized_visitor<Visitor,T,TT...>
{
public:
	specialized_visitor(T& i_arg,specialized_visitor<Visitor,TT...>& i_prevVisitor);
	template<typename ... Args>
	inline void operator()(Args&& ... i_args);

private:
	T& m_arg;
	specialized_visitor<Visitor,TT...>& m_prev;
};
template<typename T, typename Visitor, typename ... TT>
specialized_visitor(T& i_arg,specialized_visitor<Visitor,TT...>&) -> specialized_visitor<Visitor,T,TT...>;

template<typename Visitor>
struct specialized_visitor<Visitor>
{
	typedef typename mpl::static_if<std::is_void<typename Visitor::return_type>::value,void_t,typename Visitor::return_type>::type private_return_type;

public:
	typedef typename Visitor::return_type return_type;

	specialized_visitor(Visitor& i_visitor);
	specialized_visitor(const specialized_visitor&);
	template<typename ... Args>
	inline void operator()(Args&& ... i_args);
	return_type forward_return() &&;

private:
	Visitor& m_visitor;
	typed_arena<private_return_type> m_return;
};
template<typename Visitor>
specialized_visitor(Visitor& i_visitor) -> specialized_visitor<Visitor>;

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

template<typename,typename,typename,typename,size_t>
class dynamic_multi_visitor;

template<typename Visitor,typename ... Types,typename ... ResolvedTypes, typename Interface, size_t Dim>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>
	: public dynamic_visitor<typename Visitor::type_interface>
	, public detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>,Visitor,Types> ...
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(detail::specialized_visitor<Visitor,ResolvedTypes...> i_visitor);

	template<typename Value,typename ... Values>
	void visit(Value i_value, Values ... i_values);
	template<typename T>
	inline void typed_visit(T&& i_resolvedValue);
	return_type forward_return() &&;

private:
	template<size_t ... IndexsToResolve, typename T>
	inline void typed_visit(const mpl::sequence<IndexsToResolve...>&,T&&);

	detail::specialized_visitor<Visitor,ResolvedTypes...> m_visitor;
	std::array<lent_object<Interface>,Dim-1> m_pendingValues;
};

template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface>
class dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,0>
{
public:
	typedef Visitor visitor_interface;
	typedef typename Visitor::return_type return_type;

	dynamic_multi_visitor(detail::specialized_visitor<Visitor,ResolvedTypes...> i_visitor);
	void visit();

private:
	detail::specialized_visitor<Visitor,ResolvedTypes...> m_visitor;
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
