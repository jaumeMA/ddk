//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_allocator_concepts.h"
#include "ddk_type_concepts.h"

namespace ddk
{

//impl
template<typename Object,typename Return,typename ... Types>
constexpr inline detail::relative_function_impl<Object,Return,Types...> make_member_function(Object* i_object,Return(Object::*i_funcPtr)(Types...));
template<typename Object,typename Return,typename ... Types>
constexpr inline detail::relative_function_impl<const Object,Return,Types...> make_member_function(const Object* i_object,Return(Object::*i_funcPtr)(Types...)const);
template<typename Return,typename ... Types>
constexpr inline detail::free_function_impl<Return,Types...> make_free_function(Return(*i_funcPtr)(Types...));
TEMPLATE(typename Functor)
REQUIRES(IS_CLASS(Functor),IS_CALLABLE(Functor))
constexpr inline detail::resolved_functor_impl<Functor> make_functor_function(Functor&& i_functor);

//no allocator specified, no args specified
template<typename Object, typename Return, typename ... Types>
constexpr inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Object, typename Return, typename ... Types>
constexpr inline function<Return(Types...)> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const);
template<typename Return, typename ... Types>
constexpr inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
TEMPLATE(typename Functor)
REQUIRES(IS_CLASS(mpl::remove_qualifiers<Functor>),IS_CALLABLE(mpl::remove_qualifiers<Functor>))
constexpr inline detail::resolved_callable<Functor> make_function(Functor&&);

//allocator specified, no args specified
TEMPLATE(typename Object,typename Return,typename ... Types,typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline function<Return(Types...),Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
TEMPLATE(typename Object,typename Return,typename ... Types,typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline function<Return(Types...),Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator);
TEMPLATE(typename Return, typename ... Types, typename Allocator)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline function<Return(Types...),Allocator> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator);
TEMPLATE(typename Functor, typename Allocator)
REQUIRES(IS_CLASS(mpl::remove_qualifiers<Functor>),IS_CALLABLE(mpl::remove_qualifiers<Functor>),IS_ALLOCATOR(Allocator))
constexpr inline detail::resolved_callable<Functor,Allocator> make_function(Functor&&, const Allocator& i_allocator);

//no allocator specified, args specified
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRES(IS_NOT_ALLOCATOR(Arg))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Functor, typename Arg, typename ... Args)
REQUIRES(IS_CLASS(mpl::remove_qualifiers<Functor>),IS_NOT_FUNCTION(mpl::remove_qualifiers<Functor>),IS_CALLABLE(mpl::remove_qualifiers<Functor>),IS_NOT_ALLOCATOR(Arg))
constexpr inline detail::resolved_spec_callable<Functor,system_allocator,Arg,Args...> make_function(Functor&&, Arg&& i_arg, Args&& ... i_args);

//allocator specified, args specified
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args)
REQUIRES(IS_ALLOCATOR(Allocator))
constexpr inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
TEMPLATE(typename Functor, typename Allocator, typename Arg, typename ... Args)
REQUIRES(IS_CLASS(mpl::remove_qualifiers<Functor>),IS_NOT_FUNCTION(mpl::remove_qualifiers<Functor>),IS_CALLABLE(mpl::remove_qualifiers<Functor>))
constexpr inline detail::resolved_spec_callable<Functor,Allocator,Arg,Args...> make_function(Functor&&, const Allocator&, Arg&& i_arg, Args&& ... i_args);

//safe version
template<typename Return, typename Allocator, typename FunctionImpl>
constexpr inline Return eval(const detail::function_impl<Return(),Allocator,FunctionImpl>& i_function);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,function_arguments<Args...>& i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args);
TEMPLATE(typename Return, typename ... Types, typename Allocator, typename FunctionImpl, typename ... Args)
REQUIRES_COND(mpl::is_function_argument<Args>::value==false ...)
constexpr inline Return eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function, Args&& ... i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,Args&& ... i_args);
template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl, typename ... Args>
constexpr inline Return terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function, function_arguments<Args...>& i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,function_arguments<Args...>&& i_args);
TEMPLATE(typename Function, typename ... Args)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline mpl::aqcuire_callable_return_type_at<Function,Args...> eval(Function&& i_function, Args&& ... i_args);
TEMPLATE(typename Function,typename ... Args)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline mpl::aqcuire_callable_return_type_at<Function,Args...> terse_eval(Function&& i_function, Args&& ... i_args);
TEMPLATE(typename Function,typename ... Args)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline mpl::aqcuire_callable_return_type_at<Function,Args...> terse_eval(Function&& i_function,function_arguments<Args...>& i_args);
TEMPLATE(typename Function,typename ... Args)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline mpl::aqcuire_callable_return_type_at<Function,Args...> terse_eval(Function&& i_function,const function_arguments<Args...>& i_args);
TEMPLATE(typename Function,typename ... Args)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline mpl::aqcuire_callable_return_type_at<Function,Args...> terse_eval(Function&& i_function,function_arguments<Args...>&& i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args,size_t ... Indexs>
constexpr inline Return _terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,function_arguments<Args...>& i_args,const mpl::sequence<Indexs...>&);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args,size_t ... Indexs>
constexpr inline Return _terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args,const mpl::sequence<Indexs...>&);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args,size_t ... Indexs>
constexpr inline Return _terse_eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,function_arguments<Args...>&& i_args,const mpl::sequence<Indexs...>&);
TEMPLATE(typename Function,typename ... Args,size_t ... Indexs)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline auto _terse_eval(Function&& i_function,function_arguments<Args...>& i_args,const mpl::sequence<Indexs...>&);
TEMPLATE(typename Function,typename ... Args,size_t ... Indexs)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline auto _terse_eval(Function&& i_function,const function_arguments<Args...>& i_args,const mpl::sequence<Indexs...>&);
TEMPLATE(typename Function,typename ... Args, size_t ... Indexs)
REQUIRES(IS_NOT_FUNCTION(Function),IS_CALLABLE_BY(Function,Args...))
constexpr inline auto _terse_eval(Function&& i_function,function_arguments<Args...>&& i_args, const mpl::sequence<Indexs...>&);

//unsafe version
template<typename Return,typename Allocator, typename FunctionImpl>
constexpr inline Return eval_unsafe(const detail::function_impl<Return(),Allocator,FunctionImpl>& i_function);
TEMPLATE(typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args)
REQUIRES_COND(mpl::is_function_argument<Args>::value==false ...)
constexpr inline Return eval_unsafe(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,Args&& ... i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return eval_unsafe(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,function_arguments<Args...>& i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
constexpr inline Return eval_unsafe(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args);

}

#include "ddk_intersection_function.h"
#include "ddk_union_function.h"
#include "ddk_composed_function.h"
#include "ddk_specialized_callable.h"

namespace ddk
{

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> specialize(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,Args&& ... i_args);
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
inline detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> specialize(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args);
TEMPLATE(typename Functor, typename ... Args)
REQUIRES(IS_CALLABLE(Functor))
inline specialized_callable<typename std::remove_reference<Functor>::type,Args...> specialize(Functor&& i_functor, Args&& ... i_args);

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
inline function_view<Return(Types...)> lend(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function);

template<typename ... Callables>
inline detail::intersection_function<Callables...> make_intersection(const Callables& ... i_callables);

template<typename ... Callables>
inline detail::union_function<Callables...> make_union(const Callables& ... i_callables);

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
inline detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> make_composition(const function<ReturnDst(TypesDst...)>& i_fuscDst, const function<ReturnSrc(TypesSrc...)>& i_funcSrc);

TEMPLATE(typename ... Functions)
REQUIRES(IS_CALLABLE(Functions)...)
inline detail::intersection_function<Functions...> fusion(const Functions& ... i_functions);

TEMPLATE(typename ... Functions)
REQUIRES(IS_CALLABLE(Functions)...)
inline detail::union_function<Functions...> concat(const Functions& ... i_functions);

}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
inline ddk::function<ReturnA(TypesB...)> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::detail::resolved_function<ReturnA,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::intersection_function<CallablesB...>& i_rhs);
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
inline ddk::detail::resolved_function<ReturnA,typename ddk::detail::union_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::union_function<CallablesB...>& i_rhs);

#include "ddk_function_utils.inl"
