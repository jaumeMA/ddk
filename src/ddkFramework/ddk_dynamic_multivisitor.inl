
#include "ddk_embedded_type.h"
#include "ddk_dynamic_callable.h"

namespace ddk
{
namespace detail
{
namespace
{

template<typename T>
inline auto resolve_lent(const lent_value<T>& i_value)
{
	return i_value;
}
template<typename T>
inline auto resolve_lent(const lent_object<T>& i_value)
{
	return i_value;
}
TEMPLATE(typename Value)
REQUIRES(IS_INHERITED_VALUE(Value))
auto resolve_lent(const Value& i_value)
{
	return ddk::lend(i_value);
}

template<typename Value>
using resolved_lent_type = decltype(resolve_lent(std::declval<Value>()));

}

template<typename Callable,typename,typename>
struct resolve_callable_return_type;

template<typename Callable, size_t ... Indexs, typename ... Types>
struct resolve_callable_return_type<Callable,mpl::sequence<Indexs...>,mpl::type_pack<Types...>>
{
	typedef decltype(std::declval<typename std::remove_reference<Callable>::type>()(std::declval<typename mpl::type_pack<Types...>::template nth_type<mpl::index_to_index<Indexs,0>>>()...)) type;
};

template<typename FinalVisitorType, typename VisitorType>
bool __expand_type_visitor_layout()
{
	typedef typename VisitorType::type visitor_type;
	typedef typename VisitorType::type_interface type_interface;
	static const FinalVisitorType* s_finalType = reinterpret_cast<const FinalVisitorType*>(0xFFFF);
	static const typed_dynamic_visitor<visitor_type>* s_partialType = s_finalType;
	static const rtti::TypeInfo typeInfo = rtti::type_info<visitor_type,type_interface>();

	dynamic_visitor<type_interface>::add_dynamic_visitor(typeInfo,reinterpret_cast<size_t>(s_partialType) - reinterpret_cast<size_t>(s_finalType));

	return true;
}

template<typename,typename...>
struct specialized_visitor;

template<typename Visitor,typename T,typename ... TT>
specialized_visitor<Visitor,T,TT...>::specialized_visitor(T& i_arg,specialized_visitor<Visitor,TT...>& i_prevVisitor)
: m_arg(i_arg)
, m_prev(i_prevVisitor)
{
}
template<typename Visitor,typename T,typename ... TT>
template<typename ... Args>
void specialized_visitor<Visitor,T,TT...>::operator()(Args&& ... i_args)
{
	m_prev(m_arg,std::forward<Args>(i_args)...);
}

template<typename Visitor>
specialized_visitor<Visitor>::specialized_visitor(const specialized_visitor& other)
: m_visitor(other.m_visitor)
{
}
template<typename Visitor>
specialized_visitor<Visitor>::specialized_visitor(Visitor& i_visitor)
: m_visitor(i_visitor)
{
}
template<typename Visitor>
template<typename ... Args>
void specialized_visitor<Visitor>::operator()(Args&& ... i_args)
{
	if constexpr (std::is_void<typename Visitor::return_type>::value)
	{
		eval(m_visitor,std::forward<Args>(i_args)...);
	}
	else
	{
		m_return.template construct<return_type>(eval(m_visitor,std::forward<Args>(i_args)...));
	}
}
template<typename Visitor>
typename specialized_visitor<Visitor>::return_type specialized_visitor<Visitor>::forward_return() &&
{
	if constexpr (std::is_void<return_type>::value == false)
	{
		return std::move(m_return).template extract<return_type>();
	}
}


template<typename MultiVisitor,typename Visitor,typename Type>
const bool dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::__register_type_visitor = __expand_type_visitor_layout<MultiVisitor,dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>>();

template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(Type& i_value)
{
	static_cast<MultiVisitor*>(this)->typed_visit(i_value);
}
template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(const Type& i_value)
{
	static_cast<MultiVisitor*>(this)->typed_visit(i_value);
}

}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface,size_t Dim>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::dynamic_multi_visitor(detail::specialized_visitor<Visitor,ResolvedTypes...> i_visitor)
: m_visitor(i_visitor)
{
	static const bool __value = (detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>,Visitor,Types>::__register_type_visitor && ...);
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface,size_t Dim>
template<typename Value,typename ... Values>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::visit(Value i_value, Values ... i_values)
{
	m_pendingValues = {i_values...};

	i_value.template visit<typename Visitor::type_interface>(*this);
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface,size_t Dim>
template<typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::typed_visit(T&& i_resolvedValue)
{
	typedef typename mpl::make_sequence<0,Dim-1>::type indexs_to_resolve;

	typed_visit(indexs_to_resolve{},std::forward<T>(i_resolvedValue));
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface,size_t Dim>
template<size_t ... IndexsToResolve,typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::typed_visit(const mpl::sequence<IndexsToResolve...>&,T&& i_resolvedValue)
{
	typedef typename std::remove_reference<T>::type resolved_type;
    typedef dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<resolved_type,ResolvedTypes...>,Interface,Dim-1> sub_visitor_t;

    auto specVisitor = detail::specialized_visitor{i_resolvedValue,m_visitor};

	sub_visitor_t tmpVisitor(std::move(specVisitor));

	tmpVisitor.visit(m_pendingValues[IndexsToResolve]...);
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface,size_t Dim>
typename dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::return_type dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,Dim>::forward_return() &&
{
	if constexpr (std::is_void<return_type>::value == false)
	{
		return std::move(m_visitor).forward_return();
	}
}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes, typename Interface>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,0>::dynamic_multi_visitor(detail::specialized_visitor<Visitor,ResolvedTypes...> i_visitor)
: m_visitor(i_visitor)
{
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Interface>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Interface,0>::visit()
{
	m_visitor();
}

TEMPLATE(typename Callable,typename ... Values)
REQUIRED(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(Callable&& i_callable, const Values& ... i_values)
{
	typedef mpl::reduce_to_common_type<mpl::remove_qualifiers<typename Values::value_type>...> type_interface;
	static const bool s_typeExpanded = rtti::inherited_type_expansion<type_interface>;
	typedef rtti::inherited_type_list<type_interface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;
	auto _visitor = dynamic_callable<return_type,type_interface>(i_callable);

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::empty_type_pack,type_interface,mpl::num_types<Values...>> multiVisitor(_visitor);

	multiVisitor.visit(detail::resolve_lent(i_values) ...);

	if constexpr(std::is_void<return_type>::value == false)
	{
		return std::move(multiVisitor).forward_return();
	}
}

TEMPLATE(typename Return,typename Callable,typename ... Values)
REQUIRED(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(Callable&& i_callable, const Values& ... i_values)
{
	typedef mpl::reduce_to_common_type<mpl::remove_qualifiers<typename Values::value_type>...> type_interface;
	static const bool s_typeExpanded = rtti::inherited_type_expansion<type_interface>;
	typedef rtti::inherited_type_list<type_interface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;

	auto _visitor = dynamic_callable<Return,type_interface>(i_callable);

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::empty_type_pack,type_interface,mpl::num_types<Values...>> multiVisitor(_visitor);

	multiVisitor.visit(detail::resolve_lent(i_values) ...);

	if constexpr(std::is_void<return_type>::value == false)
	{
		return std::move(multiVisitor).forward_return();
	}
}

TEMPLATE(typename Callable,typename ... Values)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(const Values& ... i_values)
{
	typedef mpl::reduce_to_common_type<mpl::remove_qualifiers<typename Values::value_type>...> type_interface;
	static const bool s_typeExpanded = rtti::inherited_type_expansion<type_interface>;
	typedef rtti::inherited_type_list<type_interface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;

	auto _visitor = dynamic_callable<return_type,type_interface>(Callable{});

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::empty_type_pack,type_interface,mpl::num_types<Values...>> multiVisitor(_visitor);

	multiVisitor.visit(detail::resolve_lent(i_values) ...);

	if constexpr(std::is_void<return_type>::value == false)
	{
		return std::move(multiVisitor).forward_return();
	}
}

TEMPLATE(typename Return,typename Callable,typename ... Values)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(const Values& ... i_values)
{
	typedef mpl::reduce_to_common_type<mpl::remove_qualifiers<typename Values::value_type>...> type_interface;
	static const bool s_typeExpanded = rtti::inherited_type_expansion<type_interface>;
	typedef rtti::inherited_type_list<type_interface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;

	auto _visitor = dynamic_callable<Return,type_interface>(Callable{});

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::empty_type_pack,type_interface,mpl::num_types<Values...>> multiVisitor(_visitor);

	multiVisitor.visit(detail::resolve_lent(i_values) ...);

	if constexpr(std::is_void<return_type>::value == false)
	{
		return std::move(multiVisitor).forward_return();
	}
}

}
