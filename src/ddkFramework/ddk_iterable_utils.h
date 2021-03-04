#pragma once

#include "ddk_iterable.h"
#include "ddk_union_iterable.h"
#include "ddk_intersection_iterable.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_concepts.h"
#include "ddk_iterable_concepts.h"

namespace ddk
{

template<typename Iterable>
using resolved_iterable = detail::iterable_correspondence<Iterable>;
template<typename Iterable>
using resolved_iterable_traits = typename detail::iterable_correspondence<Iterable>::traits;
template<typename Iterable, typename T>
using resolved_iterable_traits_as = typename detail::iterable_correspondence<Iterable>::traits::template as<T>;
template<typename Iterable>
using resolved_iterable_action = typename detail::iterable_action_correspondence<Iterable>;

template<typename Iterable, typename IIterable>
inline Iterable make_iterable(IIterable&& i_iterable);

TEMPLATE(typename Iterable)
REQUIRES(IS_BASE_OF_ITERABLE(Iterable))
inline Iterable deduce_iterable(Iterable&& i_iterable);

TEMPLATE(typename Container)
REQUIRES(IS_NOT_BASE_OF_ITERABLE(Container))
inline resolved_iterable<Container> deduce_iterable(Container&& i_iterable);

template<typename T>
using deduced_iterable = decltype(deduce_iterable(std::declval<const T&>()));

template<typename>
struct transformed_traits_resolver;
template<template<typename> typename Traits>
struct transformed_traits_resolver<Traits<void>>
{
};
template<template<typename> typename Traits, typename Return>
struct transformed_traits_resolver<Traits<Return>>
{
    typedef typename mpl::static_if<std::is_reference<Return>::value,Traits<typename std::remove_reference<Return>::type>,Traits<typename std::add_const<Return>::type>>::type type;
};

template<typename Traits>
using transformed_traits = typename transformed_traits_resolver<Traits>::type;

}

//for regular iterables
template<typename Return, typename Type, typename Allocator, template<typename> typename Traits, typename IterableValue>
inline ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>> operator<<=(const ddk::detail::iterable_transform<ddk::function<Return(Type),Allocator>>& i_lhs, const ddk::detail::iterable<Traits<IterableValue>>& i_rhs);

template<typename Type, typename Allocator, typename Traits>
inline ddk::detail::iterable<Traits> operator<<=(const ddk::detail::iterable_filter<ddk::function<bool(Type),Allocator>>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

template<typename T, typename Traits>
inline ddk::detail::iterable<Traits> operator<<=(const ddk::detail::iterable_order<T>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline typename ddk::mpl::static_if<std::is_base_of<ddk::detail::iterable_value_base,IterableValue>::value,ddk::co_iteration<Traits>,ddk::iteration<Traits>>::type operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline typename ddk::mpl::static_if<std::is_base_of<ddk::detail::iterable_value_base,IterableValue>::value,ddk::co_iteration<Traits>,ddk::iteration<Traits>>::type operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

//for containers
TEMPLATE(typename Return,typename Type,typename Allocator, typename Container)
REQUIRES(IS_NOT_BASE_OF_ITERABLE(Container))
inline ddk::detail::iterable<ddk::transformed_traits<ddk::resolved_iterable_traits_as<Container,Return>>> operator<<=(const ddk::detail::iterable_transform<ddk::function<Return(Type),Allocator>>& i_lhs, Container& i_rhs);

TEMPLATE(typename Type,typename Allocator,typename Container)
REQUIRES(IS_NOT_BASE_OF_ITERABLE(Container))
inline ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_filter<ddk::function<bool(Type),Allocator>>& i_lhs,Container& i_rhs);

TEMPLATE(typename T,typename Container)
REQUIRES(IS_NOT_BASE_OF_ITERABLE(Container))
inline ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_order<T>& i_lhs,Container& i_rhs);

TEMPLATE(typename Function,typename Container)
REQUIRES(IS_CALLABLE(Function),IS_NOT_BASE_OF_ITERABLE(Container))
inline void operator<<=(Function&& i_lhs, Container& i_rhs);

namespace ddk
{

TEMPLATE(typename ... Iterables)
REQUIRES(IS_NOT_CALLABLE(Iterables)...)
inline detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> concat(const Iterables& ... i_iterables);

TEMPLATE(typename ... Iterables)
REQUIRES(IS_NOT_CALLABLE(Iterables)...)
inline detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> fusion(const Iterables& ... i_iterables);

}

#include "ddk_iterable_utils.inl"
#include "ddk_iterable_algorithm.h"