#pragma once

#include "ddk_iteration.h"
#include "ddk_union_iterable.h"
#include "ddk_intersection_iterable.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"

namespace ddk
{

template<typename Iterable, typename IIterable>
inline Iterable make_iterable(IIterable&& i_iterable);

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

namespace ddk
{

template<typename ... Traits>
inline detail::iterable<detail::union_iterable_traits<Traits...>> concat(const ddk::detail::iterable<Traits>& ... i_iterables);

template<typename ... Traits>
inline detail::iterable<detail::intersection_iterable_traits<Traits...>> fusion(const ddk::detail::iterable<Traits>& ... i_iterables);

}

#include "ddk_iterable_utils.inl"
