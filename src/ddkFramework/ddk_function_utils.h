#pragma once

namespace ddk
{

//no allocator specified, no args specified
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...)const);
template<typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
template<typename Functor>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_function(Functor&&);

//allocator specified, no args specified
template<typename Object, typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Object, typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator);
template<typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Functor, typename Allocator>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_function(Functor&&, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type* = nullptr);

//no allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,typename std::enable_if<mpl::is_allocator<Arg>::value==false,Arg>::type,Args...> make_function(Functor&&, Arg&& i_arg, Args&& ... i_args);

//allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Allocator, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator,Arg,Args...> make_function(Functor&&, const Allocator&, Arg&& i_arg, Args&& ... i_args);

template<typename Return, typename ... Types, typename ... Args>
inline Return eval(const function<Return(Types...)>& i_function, Args&& ... i_args);

template<typename RReturn, typename ... TTypes, typename AAllocator>
inline function_view<RReturn(TTypes...)> lend(const function<RReturn(TTypes...),AAllocator>&);

}

#include "ddk_function_utils.inl"
