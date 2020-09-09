
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
template<typename ... Args>
function_impl_base<Return, tuple<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::specialized_impl(const function_base_const_shared_ref<Return,tuple<Types...>>& i_object, const tuple<Args...>& i_args)
: m_object(i_object)
, m_specArgs(i_args.template get<specIndexs>() ...)
{
}
template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
Return function_impl_base<Return, tuple<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::operator()(typename mpl::nth_type_of<notSpecIndexs, Types...>::type ... i_args) const
{
    typedef tuple<typename mpl::nth_type_of<notSpecIndexs, Types...>::type ...> args_tuple;
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

	//at some point, avoid this incomingTuple construction and merge directly with args
	auto incomingTuple = args_tuple{std::forward<typename mpl::nth_type_of<notSpecIndexs, Types...>::type>(i_args) ...};
	auto mergedTuple = merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,Types...>::type ...>(m_specArgs,incomingTuple,total_indexs{},inverse_total_indexs{});

    return m_object->apply(mergedTuple);
}
template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
Return function_impl_base<Return, tuple<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::apply(vars_tuple& i_tuple) const
{
    typedef tuple<typename mpl::nth_type_of<notSpecIndexs, Types...>::type ...> args_tuple;
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

	auto mergedTuple = merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,Types...>::type ...>(m_specArgs,i_tuple,total_indexs{},inverse_total_indexs{});

    return m_object->apply(mergedTuple);
}

template<typename Return, typename ... Types>
template<typename Allocator, typename ... Args>
function_base_const_shared_ref<Return,unresolved_types<tuple<Args...>,Types...>> function_impl_base<Return, tuple<Types...>>::specialize(const Allocator& i_allocator, Args&& ... args) const
{
    typedef typename mpl::pos_place_holder<0,tuple<Args...>>::type not_spec_indexs;
    typedef typename mpl::dual_sequence<not_spec_indexs>::template at<0,s_numTypes>::type spec_sequence;
    typedef typename not_spec_indexs::template at<typename mpl::sequence_place_holder<tuple<Args...>>::type>::type not_spec_sequence;
    typedef typename mpl::make_tuple<Types...>::template at<not_spec_sequence>::type not_spec_types;
    typedef specialized_impl<spec_sequence,not_spec_sequence> spec_func_type;

    if(void* mem = i_allocator.allocate(1,sizeof(spec_func_type)))
    {
        spec_func_type* newFuncImpl = new(mem) spec_func_type(as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded)),tuple<Args...>{ std::forward<Args>(args)... } );

        return as_shared_reference(newFuncImpl,tagged_pointer<shared_reference_counter>(&(newFuncImpl->m_refCounter),ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(i_allocator));
    }
    else
    {
        throw std::exception{};
    }
}

template<typename ObjectType, typename Return, typename ... Types>
relative_function_impl<ObjectType,Return,Types...>::relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer)
: m_object(i_object)
, m_funcPointer(i_funcPointer)
{
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::operator()(Types ... args) const
{
    return (m_object->*m_funcPointer)(std::forward<Types>(args)...);
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::apply(tuple<Types...>& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    return apply(types_sequence{},i_tuple);
}
template<typename ObjectType, typename Return, typename ... Types>
template<size_t ... Indexs>
Return relative_function_impl<ObjectType,Return,Types...>::apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const
{
    return (m_object->*m_funcPointer)(i_tuple.template get<Indexs>()...);
}

template<typename Return, typename ... Types>
free_function_impl<Return,Types...>::free_function_impl(FuncPointerType i_funcPointer)
: m_funcPointer(i_funcPointer)
{
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::operator()(Types ... args) const
{
    return (*m_funcPointer)(std::forward<Types>(args)...);
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::apply(tuple<Types...>& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    return apply(types_sequence{},i_tuple);
}
template<typename Return, typename ... Types>
template<size_t ... Indexs>
Return free_function_impl<Return,Types...>::apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const
{
    return (*m_funcPointer)(i_tuple.template get<Indexs>()...);
}

template<typename T, typename Return, typename ... Types>
functor_impl<T,Return,Types...>::functor_impl(const T& i_functor)
: m_functor(i_functor)
{
}
template<typename T, typename Return, typename ... Types>
Return functor_impl<T,Return,Types...>::operator()(Types ... args) const
{
    return m_functor(std::forward<Types>(args)...);
}
template<typename T, typename Return, typename ... Types>
Return functor_impl<T,Return,Types...>::apply(tuple<Types...>& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    return apply(types_sequence{},i_tuple);
}
template<typename T, typename Return, typename ... Types>
template<size_t ... Indexs>
Return functor_impl<T,Return,Types...>::apply(const mpl::sequence<Indexs...>&, tuple<Types...>& i_tuple) const
{
    return m_functor(i_tuple.template get<Indexs>()...);
}

}
}
