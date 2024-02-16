#pragma once

#include "ddk_iterator_concepts.h"
#include "ddk_container_concepts.h"

#define IS_ITERABLE_TYPE_COND(_TYPE) \
    ddk::concepts::is_iterable_type<ddk::mpl::remove_qualifiers<_TYPE>>::value

#define IS_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_ITERABLE_TYPE_COND(_TYPE)>::type

#define IS_NOT_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_ITERABLE_TYPE_COND(_TYPE) == false>::type

#define IS_CONST_ITERABLE_COND(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE) && (IS_TYPE_CONST_COND(_TYPE) || IS_CONST_ITERATOR_COND(typename _TYPE::iterator))

#define IS_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<IS_CONST_ITERABLE_COND(_TYPE)>::type

#define IS_NON_CONST_ITERABLE_COND(_TYPE) \
    HAS_ITERATOR_DEFINED_COND(_TYPE) && (IS_TYPE_CONST_COND(_TYPE) == false && IS_CONST_ITERATOR_COND(typename _TYPE::iterator) == false)

#define IS_NON_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<IS_NON_CONST_ITERABLE_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename IterableValue,typename T,size_t ... Indexs>
typename mpl::static_if<mpl::holds_type_for_some_type<std::is_constructible,IterableValue&&,typename T::template nth_type<Indexs>...>(),std::true_type,std::false_type>::type _resolve_iterable_valued_function(const mpl::sequence<Indexs...>&);
template<typename Iterable,typename Function,typename T = typename mpl::aqcuire_callable_args_type<Function>::type>
decltype(_resolve_iterable_valued_function<typename Iterable::iterable_value,T>(typename mpl::make_sequence<0,T::size()>::type{})) resolve_iterable_valued_function(const Iterable&,Function&);
template<typename Iterable,typename Function,typename = decltype(std::declval<Function>()(std::declval<typename Iterable::iterable_value>()))>
std::true_type resolve_iterable_valued_function(const Iterable&,const Function&);
template<typename ... T>
std::false_type resolve_iterable_valued_function(const T& ...);

template<typename Iterable,typename Function>
inline constexpr bool is_iterable_valued_function = decltype(resolve_iterable_valued_function(std::declval<Iterable>(),std::declval<mpl::remove_qualifiers<Function>&>()))::value;

template<typename T>
struct is_iterable_type;

template<typename T>
struct is_iterable_type<ddk::detail::iterable<T>>
{
    static const bool value = true;
};
template<typename T>
struct is_iterable_type
{
    static const bool value = false;
};

template<typename,typename>
struct iterable_action_support_impl;

template<typename Iterable>
struct iterable_action_support_impl<Iterable,begin_action_tag>
{
private:
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static mpl::type_pack<begin_action_tag> resolve(T&,const decltype(std::declval<T>().begin())*);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>(),nullptr)) type;
};

template<typename Iterable>
struct iterable_action_support_impl<Iterable,end_action_tag>
{
private:
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static mpl::type_pack<end_action_tag> resolve(T&,const decltype(std::declval<T>().end())*);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>(),nullptr)) type;
};

template<typename Iterable>
struct iterable_action_support_impl<Iterable,remove_action_tag>
{
private:
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static mpl::type_pack<remove_action_tag> resolve(T&,const decltype(std::declval<T>().erase(std::declval<typename Iterable::const_iterator>()))*);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>(),nullptr)) type;
};

template<typename Iterable>
struct iterable_action_support_impl<Iterable,mpl::template_class_holder<add_action_tag>>
{
private:
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static mpl::type_pack<add_action_tag<typename T::value_type>> resolve(T&,const decltype(std::declval<T>().insert(std::declval<typename T::const_iterator>(),std::declval<typename T::value_type&&>()))*);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>(),nullptr)) type;
};

template<typename Iterable>
struct iterable_action_support_impl<Iterable,size_action_tag>
{
private:
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static mpl::type_pack<size_action_tag> resolve(T&,const decltype(std::declval<T>().size())*);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>(),nullptr)) type;
};

template<typename Iterable, typename Action>
struct iterable_action_support_impl
{
    template<typename T,typename = typename std::enable_if<has_iterator_defined_v<T>>::type>
    static iterator_action_support<typename mpl::which_type<mpl::is_const<T>,typename T::const_iterator,typename T::iterator>::type,Action> resolve(T&);
    static mpl::empty_type_pack resolve(...);

public:
    typedef decltype(resolve(std::declval<Iterable&>())) type;
};

template<typename Iterable, typename ... Actions>
using iterable_action_support = typename mpl::merge_type_packs<typename iterable_action_support_impl<Iterable,Actions>::type...>::type;

}
}
