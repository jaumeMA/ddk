#pragma once

#include "ddk_function.h"
#include "ddk_filter_function.h"
#include "ddk_iteration.h"

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

template<typename Return, typename Type, typename Allocator, template<typename> typename Traits, typename IterableValue>
inline ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::iterable<Traits<IterableValue>>& i_rhs);

template<typename ... Types, typename Allocator, typename Traits>
inline ddk::detail::iterable<Traits> operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

template<typename Traits>
inline ddk::future<void> operator<<=(ddk::attachable<void> i_attachable, ddk::iteration<Traits> i_iteration);

template<typename Traits1, typename Traits2>
inline ddk::detail::iterable<ddk::detail::union_iterable_traits<Traits1,Traits2>> operator|(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs);

#include "ddk_iterable_utils.inl"
