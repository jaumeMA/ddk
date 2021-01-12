#pragma once

#include "ddk_arena.h"
#include "ddk_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_function_impl.h"
#include "ddk_function_view.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_arguments.h"
#include "ddk_allocator_concepts.h"
#include "ddk_system_allocator.h"
#include "ddk_global_allocators.h"

namespace ddk
{

template<typename,typename,typename>
struct get_resolved_function;
template<typename Return,typename ... Types,typename Allocator>
struct get_resolved_function<Return,tuple<Types...>,Allocator>
{
    typedef function<Return(Types...),Allocator> type;
};

template<typename Return, typename Type, typename Allocator = system_allocator>
using resolved_function = typename get_resolved_function<Return,Type,typename std::enable_if<concepts::is_allocator<Allocator>::value,Allocator>::type>::type;

template<typename Callable, typename Allocator = system_allocator>
using resolved_callable = resolved_function<typename mpl::aqcuire_callable_return_type<Callable>::return_type,typename mpl::aqcuire_callable_return_type<Callable>::args_type,Allocator>;

template<typename Callable, typename Allocator, typename ... Args>
using resolved_spec_callable = resolved_function<typename mpl::aqcuire_callable_return_type<Callable>::return_type,detail::unresolved_tuple<tuple<Args...>,typename mpl::aqcuire_callable_return_type<Callable>::args_type>,Allocator>;

template<typename Arg, typename T>
using resolved_return_type = typename std::enable_if<is_function_argument<Arg>::value==false,T>::type;

template<typename Return, typename Allocator>
class function<Return(),Allocator>
{
    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_dist_ptr<Return,tuple<>> function_base_const_dist_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
    template<typename RReturn, typename AAllocator>
    friend inline function_view<RReturn()> lend(const function<RReturn(),AAllocator>&);
    template<typename RReturn, typename AAllocator>
    friend inline RReturn eval(const function<RReturn(),AAllocator>&);
    template<typename RReturn, typename AAllocator>
    friend inline RReturn eval(const function<RReturn(),AAllocator>&, const function_arguments<>&);

public:
	typedef Return return_type;

	function();
    function(std::nullptr_t);
    function(const function& other) = default;
    function(function&& other) = default;
    template<typename AAllocator = Allocator>
    function(const function<Return(),AAllocator>& other);
    template<typename AAllocator = Allocator>
    function(function<Return(),AAllocator>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_CALLABLE_NOT_FUNCTION(T))
    function(T&& functor, const Allocator& i_allocator = Allocator());
    template<typename T>
    function(T *pRef, Return(T::*call)(), const Allocator& i_allocator = Allocator());
    template<typename T>
    function(const T *pRef, Return(T::*call)()const, const Allocator& i_allocator = Allocator());
    function(Return(*call)(), const Allocator& i_allocator = Allocator());
	~function() = default;
    function& operator=(const function& other) = default;
    function& operator=(function&& other) = default;
    function& operator=(std::nullptr_t);
	inline Return inline_eval() const;
	inline Return inline_eval(const function_arguments<>& i_args) const;
    inline bool operator==(std::nullptr_t) const;
    inline bool operator!=(std::nullptr_t) const;

private:
    inline Return eval_arguments(const mpl::sequence<>&) const;

    function_base_const_dist_ptr m_functionImpl;
	fixed_size_allocate_or<Allocator> m_allocator;
};

template<typename Return, typename ... Types, typename Allocator>
class function<Return(Types...),Allocator>
{
    template<typename,typename>
    friend class function;
    typedef detail::function_base_const_dist_ptr<Return,tuple<Types...>> function_base_const_dist_ptr;
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
    template<typename RReturn, typename ... TTypes, typename AAllocator>
    friend inline function_view<RReturn(TTypes...)> lend(const function<RReturn(TTypes...),AAllocator>& i_function);

public:
	typedef Return return_type;

	function();
    function(std::nullptr_t);
    function(const function& other) = default;
    function(function&& other) = default;
    template<typename AAllocator = Allocator>
    function(const function<Return(Types...),AAllocator>& other);
    template<typename AAllocator = Allocator>
    function(function<Return(Types...),AAllocator>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_CALLABLE_NOT_FUNCTION(T))
	function(T&& functor, const Allocator& i_allocator = Allocator());
    template<typename T>
    function(T *pRef, Return(T::*call)(Types...), const Allocator& i_allocator = Allocator());
    template<typename T>
    function(const T *pRef, Return(T::*call)(Types...)const, const Allocator& i_allocator = Allocator());
    function(Return(*call)(Types...), const Allocator& i_allocator = Allocator());
    function& operator=(const function& other) = default;
    function& operator=(function&& other) = default;
    function& operator=(std::nullptr_t);
    inline bool operator==(std::nullptr_t) const;
    inline bool operator!=(std::nullptr_t) const;
	template<typename ... Args>
	inline Return inline_eval(Args&& ... args) const;
	template<typename ... Args>
	inline Return inline_eval(const function_arguments<Args...>& i_args) const;
	template<typename ... Args>
    inline resolved_function<Return,detail::unresolved_types<tuple<Args...>,Types...>,Allocator> operator()(Args&& ... args) const;

private:
    template<size_t ... Indexs, typename ... Args>
    inline Return eval_arguments(const mpl::sequence<Indexs...>&, const function_arguments<Args...>& i_args) const;

    function_base_const_dist_ptr m_functionImpl;
	fixed_size_allocate_or<Allocator> m_allocator;
};

namespace mpl
{

template<typename Return, typename ... Types>
struct aqcuire_callable_return_type<function<Return(Types...)>>
{
	typedef Return return_type;
	typedef tuple<Types...> args_type;
};

}

//template<typename Return, typename ... Types>
//using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}

#include "ddk_function.inl"

#include "ddk_function_utils.h"
