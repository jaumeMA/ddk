#pragma once

#include "ddk_intersection_function.h"
#include "ddk_union_function.h"
#include "ddk_composed_function.h"
#include "ddk_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_allocator_concepts.h"
#include "ddk_type_concepts.h"

namespace ddk
{

//impl
template<typename Object,typename Return,typename ... Types>
inline detail::relative_function_impl<Object,Return,Types...> make_member_function(Object* i_object,Return(Object::*i_funcPtr)(Types...));
template<typename Object,typename Return,typename ... Types>
inline detail::relative_function_impl<const Object,Return,Types...> make_member_function(const Object* i_object,Return(Object::*i_funcPtr)(Types...)const);
template<typename Return,typename ... Types>
inline detail::free_function_impl<Return,Types...> make_free_function(Return(*i_funcPtr)(Types...));

//no allocator specified, no args specified
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const);
template<typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
TEMPLATE(typename Functor)
REQUIRES(IS_CLASS(Functor))
inline resolved_callable<Functor> make_function(Functor&&);

//allocator specified, no args specified
TEMPLATE(typename Object,typename Return,typename ... Types,typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
inline function<Return(Types...),Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
TEMPLATE(typename Object,typename Return,typename ... Types,typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
inline function<Return(Types...),Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator);
TEMPLATE(typename Return, typename ... Types, typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
inline function<Return(Types...),Allocator> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator);
TEMPLATE(typename Functor, typename Allocator)
REQUIRES(IS_CLASS(Functor),IS_ALLOCATOR(Allocator))
inline resolved_callable<Functor,Allocator> make_function(Functor&&, const Allocator& i_allocator);

//no allocator specified, args specified
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Functor, typename Arg, typename ... Args)
REQUIRES(IS_CLASS(Functor),IS_NOT_ALLOCATOR(Arg))
inline resolved_spec_callable<Functor,system_allocator,Arg,Args...> make_function(Functor&&, Arg&& i_arg, Args&& ... i_args);

//allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Functor, typename Allocator, typename Arg, typename ... Args)
REQUIRES(IS_CLASS(Functor))
inline resolved_spec_callable<Functor,Allocator,Arg,Args...> make_function(Functor&&, const Allocator&, Arg&& i_arg, Args&& ... i_args);

//safe version
template<typename Return, typename ... Types, typename Allocator>
inline function_view<Return(Types...)> lend(const function<Return(Types...),Allocator>& i_function);
template<typename Return, typename Allocator>
inline Return eval(const function<Return(),Allocator>& i_function);
template<typename Return, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_return_type<Arg,Return> eval(const function<Return(Types...),Allocator>& i_function, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename ... Types, typename Allocator, typename ... Args>
inline Return eval(const function<Return(Types...),Allocator>& i_function, const function_arguments<Args...>& i_args);

//unsafe version
template<typename Return,typename Allocator>
inline Return eval_unsafe(const function<Return(),Allocator>& i_function);
template<typename Return,typename ... Types,typename Allocator,typename Arg,typename ... Args>
inline resolved_return_type<Arg,Return> eval_unsafe(const function<Return(Types...),Allocator>& i_function,Arg&& i_arg,Args&& ... i_args);
template<typename Return,typename ... Types,typename Allocator,typename ... Args>
inline Return eval_unsafe(const function<Return(Types...),Allocator>& i_function,const function_arguments<Args...>& i_args);


template<typename ... Callables>
inline detail::intersection_function<Callables...> make_intersection(const Callables& ... i_callables);

template<typename ... Callables>
inline detail::union_function<Callables...> make_union(const Callables& ... i_callables);

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
inline detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> make_composition(const function<ReturnDst(TypesDst...)>& i_fuscDst, const function<ReturnSrc(TypesSrc...)>& i_funcSrc);

TEMPLATE(typename ... Functions)
REQUIRES(ARE_CALLABLES(Functions...))
inline ddk::detail::intersection_function<Functions...> fusion(const Functions& ... i_functions)
{
	return { i_functions ... };
}

TEMPLATE(typename ... Functions)
REQUIRES(ARE_CALLABLES(Functions...))
inline ddk::detail::union_function<Functions...> concat(const Functions& ... i_functions)
{
	return { i_functions ... };
}

}


template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
inline ddk::function<ReturnA(TypesB...)> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::resolved_function<ReturnA,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::intersection_function<CallablesB...>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::resolved_function<ReturnA,typename ddk::detail::union_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::union_function<CallablesB...>& i_rhs);

#include "ddk_function_utils.inl"
