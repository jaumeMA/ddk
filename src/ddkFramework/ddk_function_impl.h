#pragma once

#include "ddk_tuple.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_template_helper.h"
#include "ddk_tagged_pointer.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
struct function_impl_base;

template<typename T, typename TT>
using function_base_dist_ref = distributed_reference_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_const_dist_ref = distributed_reference_wrapper<const function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_dist_ptr = distributed_pointer_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_const_dist_ptr = distributed_pointer_wrapper<const function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_const_lent_ptr = lent_pointer_wrapper<const function_impl_base<T, TT>>;
template<typename Sequence, typename ... Types>
using types_at_indexs = typename mpl::make_tuple<Types...>::template at<Sequence>::type;
template<typename TTypes, typename ... Types>
using place_holder_types = typename mpl::make_tuple<Types...>::template at<typename mpl::pos_place_holder<0,TTypes>::type>::type;
template<typename TTypes>
using place_holders_at_indexs = typename mpl::pos_place_holder<0,TTypes>::type;
template<typename TTypes, typename ... Types>
using unresolved_types = typename mpl::make_tuple<Types...>::template at<typename place_holders_at_indexs<TTypes>::template at<typename mpl::sequence_place_holder<TTypes>::type>::type>::type;
template<typename TTypes, typename Types>
using unresolved_tuple = typename mpl::make_tuple<Types>::template at<typename place_holders_at_indexs<TTypes>::template at<typename mpl::sequence_place_holder<TTypes>::type>::type>::type;

template<typename Return, typename ... Types>
struct function_impl_base<Return, tuple<Types...>> : public distribute_from_this<function_impl_base<Return,tuple<Types...>>>
{
    static const size_t s_numTypes = mpl::get_num_types<Types...>();

    template<typename, typename>
    friend struct function_impl_base;

	template<typename,typename>
	struct specialized_impl;

	template<size_t ... specIndexs, size_t ... notSpecIndexs>
	struct specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>> : function_impl_base<Return, typename mpl::make_tuple<Types...>::template at<mpl::sequence<notSpecIndexs...>>::type>
	{
#ifndef _WIN32
		static_assert((std::is_copy_constructible<typename mpl::nth_type_of<specIndexs, Types...>::type>::value && ...), "You cannot specialize non copy constructible arguments");
#endif
        typedef types_at_indexs<mpl::sequence<notSpecIndexs...>,typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type...> vars_tuple;
        typedef types_at_indexs<mpl::sequence<specIndexs...>,typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> args_tuple;

        specialized_impl() = default;
        template<typename ... Args>
		specialized_impl(const function_base_const_dist_ref<Return,tuple<Types...>>& i_object, tuple<Args...>&& i_args);

    private:
		Return operator()(typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type ... i_args) const override;
        Return apply(const vars_tuple& i_tuple) const override;

        function_base_const_dist_ref<Return,tuple<Types...>> m_object;
		mutable args_tuple m_specArgs;
	};

    typedef tuple<typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type...> tuple_args;

	function_impl_base() = default;
	function_impl_base(const function_impl_base&) = delete;
	function_impl_base(function_impl_base&&) = default;
	virtual ~function_impl_base() = default;

	template<typename Allocator, typename ... Args>
	function_base_const_dist_ref<Return,unresolved_types<tuple<Args...>,Types...>> specialize(const Allocator& i_allocator, Args&& ... args) const;

	virtual Return operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const = 0;

private:
    virtual Return apply(const tuple_args& i_tuple) const = 0;
};

//non static member function case
template<typename ObjectType, typename Return, typename ... Types>
class relative_function_impl : public function_impl_base<Return, tuple<Types...>>
{
    typedef Return(ObjectType::*NonConstFuncPointerType)(Types...);
    typedef Return(ObjectType::*ConstFuncPointerType)(Types...)const;
    typedef typename mpl::static_if<std::is_const<ObjectType>::value,ConstFuncPointerType,NonConstFuncPointerType>::type FuncPointerType;
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;
    using typename function_impl_base<Return, tuple<Types...>>::tuple_args;

public:
	typedef Return return_type;

	relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer);
	relative_function_impl(const relative_function_impl&) = delete;
	relative_function_impl(relative_function_impl&& other);

	inline Return inline_eval(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const;
	Return operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

	ObjectType* m_object;
    FuncPointerType m_funcPointer;
};

//static member function or free function case
template<typename Return, typename ... Types>
class free_function_impl : public function_impl_base<Return, tuple<Types...>>
{
    typedef Return(*FuncPointerType)(Types...);
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;
    using typename function_impl_base<Return, tuple<Types...>>::tuple_args;

public:
	typedef Return return_type;

	free_function_impl(FuncPointerType i_funcPointer);

	inline Return inline_eval(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const;
	Return operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

    FuncPointerType m_funcPointer;
};

//functor (lambdas) case
template<typename T, typename Return, typename ... Types>
class functor_impl : public function_impl_base<Return, tuple<Types...>>
{
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;
    using typename function_impl_base<Return, tuple<Types...>>::tuple_args;

public:
	typedef Return return_type;

	functor_impl(const T& i_functor);
	functor_impl(T&& i_functor);

	inline Return inline_eval(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const;
	Return operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

    mutable T m_functor;
};

template<typename,typename,typename>
struct get_resolved_functor;
template<typename Functor, typename Return,typename ... Types>
struct get_resolved_functor<Functor,Return,tuple<Types...>>
{
	typedef functor_impl<Functor,Return,Types...> type;
};

template<typename Callable, typename Return,typename Type>
using resolved_functor = typename get_resolved_functor<Callable,Return,Type>::type;

template<typename Callable>
using resolved_functor_impl = resolved_functor<Callable,typename mpl::aqcuire_callable_return_type<Callable>::return_type,typename mpl::aqcuire_callable_return_type<Callable>::args_type>;

}
}

#include "ddk_function_impl.inl"
