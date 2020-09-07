#pragma once

#include "ddk_arena.h"
#include "ddk_function_impl.h"
#include "ddk_system_reference_wrapper_allocator.h"

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

template<typename Return, typename Type, typename Allocator>
using resolved_function = typename get_resolved_function<Return,Type,Allocator>::type;

const size_t k_embeddedStorageSize = 16;

template<typename Return, typename Allocator>
class function<Return(),Allocator>
{
    static_assert(std::is_base_of<IReferenceWrapperDeleter,Allocator>::value, "You shall provide an inherited deleter allocator");

    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_shared_ptr<Return,tuple<>> function_base_const_shared_ptr;

public:
    function() = default;
    function(std::nullptr_t);
    function(const function& other);
    function(function&& other);
    template<typename T>
    function(T&& functor, typename std::enable_if<mpl::is_valid_functor<T>::value>::type* = nullptr);
    template<typename T>
    function(T *pRef, Return(T::*call)());
    function(Return(*call)());
    function(function_base_const_shared_ptr i_functionImpl);
    function& operator=(const function& other);
    function& operator=(function&& other);
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

public:
    function() = default;
    function(std::nullptr_t);
    template<typename T>
    function(T&& functor, typename std::enable_if<mpl::is_valid_functor<T>::value>::type* = nullptr);
    template<typename T>
    function(T *pRef, Return(T::*call)(Types...));
    function(Return(*call)(Types...));
    function(function_base_const_shared_ptr i_functionImpl);
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

template<typename Object, typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...));
template<typename Return, typename ... Types>
inline function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...));
template<typename Functor>
inline typename mpl::aqcuire_callable_return_type<Functor>::func_type make_function(const Functor&);

//template<typename Return, typename ... Types>
//using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}

#include "ddk_function.inl"
