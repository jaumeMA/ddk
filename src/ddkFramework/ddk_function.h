#pragma once

#include "ddk_arena.h"
#include "ddk_function_impl.h"
#include "ddk_system_reference_wrapper_allocator.h"
#include "ddk_allocator_template_helper.h"
#include "ddk_function_view.h"

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

template<typename Return, typename Allocator>
class function<Return(),Allocator>
{
    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_shared_ptr<Return,tuple<>> function_base_const_shared_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
    template<typename RReturn, typename ... TTypes, typename AAllocator>
    friend inline function_view<RReturn(TTypes...)> lend(const function<RReturn(TTypes...),AAllocator>&);

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
    template<typename RReturn, typename ... TTypes, typename AAllocator>
    friend inline function_view<RReturn(TTypes...)> lend(const function<RReturn(TTypes...),AAllocator>&);

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

//template<typename Return, typename ... Types>
//using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}

#include "ddk_function.inl"

#include "ddk_function_utils.h"
