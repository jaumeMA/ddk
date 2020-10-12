#pragma once

#include "ddk_intersection_function.h"
#include "ddk_union_function.h"
#include "ddk_composed_function.h"

namespace ddk
{

//no allocator specified, no args specified
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const);
template<typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
template<typename Functor>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_function(Functor&&);

//allocator specified, no args specified
template<typename Object, typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Object, typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator);
template<typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Functor, typename Allocator>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_function(Functor&&, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type* = nullptr);

//no allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,typename std::enable_if<mpl::is_allocator<Arg>::value==false,Arg>::type,Args...> make_function(Functor&&, Arg&& i_arg, Args&& ... i_args);

//allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Allocator, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator,Arg,Args...> make_function(Functor&&, const Allocator&, Arg&& i_arg, Args&& ... i_args);

template<typename Return, typename ... Types, typename Allocator>
inline function_view<Return(Types...)> lend(const function<Return(Types...),Allocator>& i_function);
template<typename Return, typename Allocator>
inline Return eval(const function<Return(),Allocator>& i_function);
template<typename Return, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_return_type<Arg,Return> eval(const function<Return(Types...),Allocator>& i_function, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename ... Types, typename Allocator, typename ... Args>
inline Return eval(const function<Return(Types...),Allocator>& i_function, const function_arguments<Args...>& i_args);

template<typename ... Callables>
inline detail::intersection_function<Callables...> make_intersection(const Callables& ... i_callables);

template<typename ... Callables>
inline detail::union_function<Callables...> make_union(const Callables& ... i_callables);

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
inline detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> make_composition(const function<ReturnDst(TypesDst...)>& i_fuscDst, const function<ReturnSrc(TypesSrc...)>& i_funcSrc);

}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
inline ddk::detail::intersection_function<ddk::function<ReturnA(TypesA...)>,ddk::function<ReturnB(TypesB...)>> operator&(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
inline ddk::detail::union_function<ddk::function<ReturnA(TypesA...)>,ddk::function<ReturnB(TypesB...)>> operator|(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
inline ddk::function<ReturnA(TypesB...)> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::resolved_function<ReturnA,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::intersection_function<CallablesB...>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::resolved_function<ReturnA,typename ddk::detail::union_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::union_function<CallablesB...>& i_rhs);

#include "ddk_function_utils.inl"
