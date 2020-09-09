#pragma once

#include "ddk_arena.h"
#include "ddk_function_impl.h"
#include "ddk_system_reference_wrapper_allocator.h"
#include "ddk_allocator_template_helper.h"

namespace ddk
{

template<typename,typename = system_allocator>
class function;

template<typename,typename,typename>
struct get_resolved_function;
template<typename Return,typename ... Types,typename Allocator>
struct get_resolved_function<Return,tuple<Types...>,Allocator>
{
    typedef function<Return(Types...),Allocator> type;
};

template<typename Return, typename Type, typename Allocator = system_allocator>
using resolved_function = typename get_resolved_function<Return,Type,typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type>::type;

template<typename Callable, typename Allocator = system_allocator>
using resolved_callable = resolved_function<typename mpl::aqcuire_callable_return_type<Callable>::return_type,typename mpl::aqcuire_callable_return_type<Callable>::args_type,Allocator>;

template<typename Callable, typename Allocator, typename ... Args>
using resolved_spec_callable = resolved_function<typename mpl::aqcuire_callable_return_type<Callable>::return_type,detail::unresolved_tuple<tuple<Args...>,typename mpl::aqcuire_callable_return_type<Callable>::args_type>,Allocator>;

const size_t k_embeddedStorageSize = 16;

template<typename Return, typename Allocator>
class function<Return(),Allocator>
{
    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_shared_ptr<Return,tuple<>> function_base_const_shared_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;

public:
    function() = default;
    function(std::nullptr_t);
    function(const function& other) = default;
    function(function&& other) = default;
    template<typename T>
    function(T&& functor, const Allocator& i_allocator = Allocator(), typename std::enable_if<mpl::is_valid_functor<T>::value>::type* = nullptr);
    template<typename T>
    function(T *pRef, Return(T::*call)(), const Allocator& i_allocator = Allocator());
    function(Return(*call)(), const Allocator& i_allocator = Allocator());
    function& operator=(const function& other) = default;
    function& operator=(function&& other) = default;
    function& operator=(std::nullptr_t);
    operator Return() const;
    Return operator()() const;
    bool operator==(std::nullptr_t) const;
    bool operator!=(std::nullptr_t) const;
    Return eval() const;

private:
    function_base_const_shared_ptr m_functionImpl;
    Allocator m_allocator;
};

template<typename Return, typename ... Types, typename Allocator>
class function<Return(Types...),Allocator>
{
    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_shared_ptr<Return,tuple<Types...>> function_base_const_shared_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;

public:
    function() = default;
    function(std::nullptr_t);
    function(const function& other) = default;
    function(function&& other) = default;
    template<typename T>
    function(T&& functor, const Allocator& i_allocator = Allocator(), typename std::enable_if<mpl::is_valid_functor<T>::value>::type* = nullptr);
    template<typename T>
    function(T *pRef, Return(T::*call)(Types...), const Allocator& i_allocator = Allocator());
    function(Return(*call)(Types...), const Allocator& i_allocator = Allocator());
    function& operator=(const function& other) = default;
    function& operator=(function&& other) = default;
    function& operator=(std::nullptr_t);
    bool operator==(std::nullptr_t) const;
    bool operator!=(std::nullptr_t) const;
    template<typename ... Args>
    resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> operator()(Args&& ... args) const;
    template<typename ... Args>
    Return eval(Args&& ... i_args) const;

private:
    function_base_const_shared_ptr m_functionImpl;
    Allocator m_allocator;
};

//no allocator specified, no args specified
template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
template<typename Functor>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_function(Functor&&);

//allocator specified, no args specified
template<typename Object, typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Return, typename ... Types, typename Allocator>
inline function<Return(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Functor, typename Allocator>
inline resolved_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_function(Functor&&, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type* = nullptr);

//no allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<typename std::enable_if<mpl::is_allocator<Arg>::value == false,Arg>::type,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,system_allocator,typename std::enable_if<mpl::is_allocator<Arg>::value==false,Arg>::type,Args...> make_function(Functor&&, Arg&& i_arg, Args&& ... i_args);

//allocator specified, args specified
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
inline resolved_function<Return,detail::unresolved_types<tuple<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args);
template<typename Functor, typename Allocator, typename Arg, typename ... Args>
inline resolved_spec_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator,Arg,Args...> make_function(Functor&&, const Allocator&, Arg&& i_arg, Args&& ... i_args);

//template<typename Return, typename ... Types>
//using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}

#include "ddk_function.inl"
