
#include "ddk_embedded_type.h"

namespace ddk
{
namespace detail
{

template<typename FinalVisitorType, typename VisitorType>
bool __expand_type_visitor_layout()
{
	typedef typename VisitorType::type visitor_type;
	typedef typename VisitorType::type_interface type_interface;
	static const FinalVisitorType* s_finalType = reinterpret_cast<const FinalVisitorType*>(0xFFFF);
	static const typed_dynamic_visitor<visitor_type>* s_partialType = s_finalType;
	static const rtti::TypeInfo typeInfo = rtti::type_info<visitor_type>();

	dynamic_visitor<type_interface>::add_dynamic_visitor(typeInfo,reinterpret_cast<size_t>(s_partialType) - reinterpret_cast<size_t>(s_finalType));

	return true;
}

template<typename MultiVisitor,typename Visitor,typename Type>
const bool dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::__register_type_visitor = __expand_type_visitor_layout<MultiVisitor,dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>>();

template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(const Type& i_value)
{
	static_cast<const MultiVisitor*>(this)->typed_visit(i_value);
}
template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(const Type& i_value) const
{
	static_cast<const MultiVisitor*>(this)->typed_visit(i_value);
}

}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::dynamic_multi_visitor(Visitor& i_visitor,const inherited_value<Value>& i_value,const inherited_value<Values>& ... i_pendingValues)
: m_visitor(i_visitor)
, m_value(i_value)
, m_pendingValues(i_pendingValues...)
{
	static const bool __value = (detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>,Visitor,Types>::__register_type_visitor && ...);
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
function<typename Visitor::return_type(ResolvedTypes...)> dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::visit() const
{
	m_value.template visit<typename Visitor::type_interface>(*this);

	return m_resolvedFunction;
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
template<typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::typed_visit(T&& i_resolvedValue) const
{
	typedef typename mpl::make_sequence<0,mpl::get_num_types<ResolvedTypes...>()>::type indexs_resolved;
	typedef typename mpl::make_sequence<0,mpl::get_num_types<Values...>()>::type indexs_to_resolve;

	typed_visit(indexs_resolved{},indexs_to_resolve{},std::forward<T>(i_resolvedValue));
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
template<size_t ... IndexsResolved,size_t ... IndexsToResolve,typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::typed_visit(const mpl::sequence<IndexsResolved...>&, const mpl::sequence<IndexsToResolve...>&,T&& i_resolvedValue) const
{
	typedef typename std::add_lvalue_reference<T>::type resolved_type;
	dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...,embedded_type<resolved_type>>,Values...> tmpVisitor(m_visitor,m_pendingValues.template get<IndexsToResolve>() ...);

	const function<return_type(ResolvedTypes...,embedded_type<resolved_type>)> partialFunction = tmpVisitor.visit();

	m_resolvedFunction = partialFunction(mpl::place_holder<IndexsResolved>{}...,std::forward<T>(i_resolvedValue));
}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>::dynamic_multi_visitor(Visitor& i_visitor)
: m_visitor(i_visitor)
{
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
function<typename Visitor::return_type(ResolvedTypes...)> dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>::visit() const
{
	return make_function([thisVisitor = std::move(m_visitor)](ResolvedTypes ... i_values) mutable -> return_type { return thisVisitor(*i_values ...); });
}

TEMPLATE(typename Visitor,typename ... Values)
REQUIRED(IS_BASE_OF_DYNAMIC_VISITOR(Visitor),IS_INHERITED_VALUE(Values)...)
typename std::remove_reference<Visitor>::type::return_type visit(Visitor&& i_visitor,const Values& ... i_values)
{
    typedef typename std::remove_reference<Visitor>::type visitor_t;
	typedef typename visitor_t::type_interface type_interface;
	static const bool s_typeExpanded = rtti::inherited_type_expansion<type_interface>;

	dynamic_multi_visitor<visitor_t,rtti::inherited_type_list<type_interface>,mpl::type_pack<>,typename Values::value_type...> multiVisitor(i_visitor,i_values ...);

	const function<typename visitor_t::return_type()> resolvedFunc = multiVisitor.visit();

	return eval(resolvedFunc);
}

}
