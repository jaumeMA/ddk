
#include "ddk_reference_wrapper.h"
#include "ddk_function_exceptions.h"
#include "ddk_allocator_exceptions.h"

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
Return function_impl_base<Return, tuple<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::operator()(typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type ... i_args) const
{
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

    if constexpr (std::is_same<Return,void>::value)
    {
        m_object->apply(merge_args<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,std::forward<typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type>(i_args) ...));
    }
    else
    {
        return m_object->apply(merge_args<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,std::forward<typename mpl::static_if<std::is_copy_constructible<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::value,typename mpl::nth_type_of<notSpecIndexs,Types...>::type,typename std::add_rvalue_reference<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>::type>::type>(i_args) ...));
    }
}
template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
Return function_impl_base<Return, tuple<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::apply(const vars_tuple& i_tuple) const
{
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

    if constexpr (std::is_same<Return,void>::value)
    {
        m_object->apply(merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type...>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,i_tuple));
    }
    else
    {
        return m_object->apply(merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type...>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,i_tuple));
    }
}

template<typename Return, typename ... Types>
template<typename Allocator, typename ... Args>
function_base_const_shared_ref<Return,unresolved_types<tuple<Args...>,Types...>> function_impl_base<Return, tuple<Types...>>::specialize(const Allocator& i_allocator, Args&& ... args) const
{
    typedef typename mpl::pos_place_holder<0,tuple<Args...>>::type not_spec_indexs;
    typedef typename mpl::dual_sequence<not_spec_indexs>::template at<0,s_numTypes>::type spec_sequence;
    typedef typename not_spec_indexs::template at<typename mpl::sequence_place_holder<tuple<Args...>>::type>::type not_spec_sequence;
    typedef specialized_impl<spec_sequence,not_spec_sequence> spec_func_type;

    if(void* mem = i_allocator.allocate(1,sizeof(spec_func_type)))
    {
        spec_func_type* newFuncImpl = new(mem) spec_func_type(as_shared_reference(this,tagged_pointer<shared_reference_counter>(&m_refCounter,ReferenceAllocationType::Embedded)),make_tuple(std::forward<Args>(args)...));

        return as_shared_reference(newFuncImpl,tagged_pointer<shared_reference_counter>(&(newFuncImpl->m_refCounter),ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(i_allocator));
    }
    else
    {
        throw bad_allocation_exception{};
    }
}

template<typename ObjectType, typename Return, typename ... Types>
relative_function_impl<ObjectType,Return,Types...>::relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer)
: m_object(i_object)
, m_funcPointer(i_funcPointer)
{
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (m_object->*m_funcPointer)(std::forward<typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type>(args)...);
    }
    else
    {
        return (m_object->*m_funcPointer)(std::forward<typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type>(args)...);
    }
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename ObjectType, typename Return, typename ... Types>
template<size_t ... Indexs>
Return relative_function_impl<ObjectType,Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (m_object->*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return (m_object->*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

template<typename Return, typename ... Types>
free_function_impl<Return,Types...>::free_function_impl(FuncPointerType i_funcPointer)
: m_funcPointer(i_funcPointer)
{
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (*m_funcPointer)(std::forward<typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type>(args)...);
    }
    else
    {
        return (*m_funcPointer)(std::forward<typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type>(args)...);
    }
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename Return, typename ... Types>
template<size_t ... Indexs>
Return free_function_impl<Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return (*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

template<typename T, typename Return, typename ... Types>
functor_impl<T,Return,Types...>::functor_impl(const T& i_functor)
: m_functor(i_functor)
{
}
template<typename T, typename Return, typename ... Types>
functor_impl<T,Return,Types...>::functor_impl(T&& i_functor)
: m_functor(std::move(i_functor))
{
}
template<typename T, typename Return, typename ... Types>
Return functor_impl<T,Return,Types...>::operator()(typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        m_functor(std::forward<decltype(args)>(args)...);
    }
    else
    {
        return m_functor(std::forward<decltype(args)>(args)...);
    }
}
template<typename T, typename Return, typename ... Types>
Return functor_impl<T,Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename T, typename Return, typename ... Types>
template<size_t ... Indexs>
Return functor_impl<T,Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        m_functor(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return m_functor(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

}
}
