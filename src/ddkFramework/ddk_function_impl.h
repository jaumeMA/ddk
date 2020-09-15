#pragma once

#include "ddk_tuple.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_template_helper.h"
#include "ddk_tagged_pointer.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
struct function_impl_base;

template<typename T, typename TT>
using function_base_shared_ref = shared_reference_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_const_shared_ref = shared_reference_wrapper<const function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_shared_ptr = shared_pointer_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_base_const_shared_ptr = shared_pointer_wrapper<const function_impl_base<T, TT>>;
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
struct function_impl_base<Return, tuple<Types...>>
{
    static const size_t s_numTypes = mpl::get_num_types<Types...>::value;

    template<typename, typename>
    friend struct function_impl_base;

	template<typename,typename>
	struct specialized_impl;

	template<size_t ... specIndexs, size_t ... notSpecIndexs>
	struct specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>> : function_impl_base<Return, typename mpl::make_tuple<Types...>::template at<mpl::sequence<notSpecIndexs...>>::type>
	{
        typedef types_at_indexs<mpl::sequence<notSpecIndexs...>,Types...> vars_tuple;
        typedef types_at_indexs<mpl::sequence<specIndexs...>,typename std::remove_reference<Types>::type ...> args_tuple;

        specialized_impl() = default;
        template<typename ... Args>
		specialized_impl(const function_base_const_shared_ref<Return,tuple<Types...>>& i_object, const tuple<Args...>& i_args);

    private:
		Return operator()(typename mpl::nth_type_of<notSpecIndexs, Types...>::type ... i_args) const override;
        Return apply(vars_tuple& i_tuple) const override;

        function_base_const_shared_ref<Return,tuple<Types...>> m_object;
		mutable args_tuple m_specArgs;
	};

	function_impl_base() = default;
	virtual ~function_impl_base() = default;

	template<typename Allocator, typename ... Args>
	function_base_const_shared_ref<Return,unresolved_types<tuple<Args...>,Types...>> specialize(const Allocator& i_allocator, Args&& ... args) const;

	virtual Return operator()(Types ... args) const = 0;

    mutable shared_reference_counter m_refCounter;

private:
    virtual Return apply(tuple<Types...>& i_tuple) const = 0;
};

//non static member function case
template<typename ObjectType, typename Return, typename ... Types>
class relative_function_impl : public function_impl_base<Return, tuple<Types...>>
{
    typedef Return(ObjectType::*FuncPointerType)(Types...);
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;

public:
	relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer);

private:
	Return operator()(Types ... args) const override;
    Return apply(tuple<Types...>& i_tuple) const override;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const;

	ObjectType* m_object;
    FuncPointerType m_funcPointer;
};

//static member function or free function case
template<typename Return, typename ... Types>
class free_function_impl : public function_impl_base<Return, tuple<Types...>>
{
    typedef Return(*FuncPointerType)(Types...);
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;

public:
	free_function_impl(FuncPointerType i_funcPointer);

private:
	Return operator()(Types ... args) const override;
    Return apply(tuple<Types...>& i_tuple) const override;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const;

    FuncPointerType m_funcPointer;
};

//functor (lambdas) case
template<typename T, typename Return, typename ... Types>
class functor_impl : public function_impl_base<Return, tuple<Types...>>
{
    using function_impl_base<Return, tuple<Types...>>::s_numTypes;

public:
	functor_impl(const T& i_functor);

private:
	Return operator()(Types ... args) const override;
    Return apply(tuple<Types...>& i_tuple) const override;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const;

    T m_functor;
};

}
}

#include "ddk_function_impl.inl"
