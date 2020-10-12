#pragma once

#include "ddk_function.h"
#include "ddk_filter_function.h"
#include "ddk_iteration.h"
#include "ddk_union_iterable.h"
#include "ddk_intersection_iterable.h"

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
inline ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::iterable<Traits<IterableValue>>& i_rhs);

template<typename ... Types, typename Allocator, typename Traits>
inline ddk::detail::iterable<Traits> operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs);

//for union iterables
template<typename Return, typename Type, typename Allocator, typename ... Iterables>
inline ddk::detail::iterable<ddk::transformed_traits<ddk::detail::union_iterable_transformed_traits<Return,Iterables...>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs);

template<typename ... Types, typename Allocator, typename ... Iterables>
inline typename ddk::detail::union_iterable<Iterables...>::related_iterable operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs);

template<typename IterableValue, typename Allocator, typename ... Iterables>
inline ddk::iteration<typename ddk::detail::union_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::union_iterable<Iterables...>& i_rhs);

template<typename IterableValue, typename Allocator, typename ... Iterables>
inline ddk::iteration<typename ddk::detail::union_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs);

//for intersection iterables
template<typename Return, typename Type, typename Allocator, typename ... Iterables>
inline ddk::detail::iterable<ddk::transformed_traits<ddk::detail::intersection_iterable_transformed_traits<Return,Iterables...>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs);

template<typename ... Types, typename Allocator, typename ... Iterables>
inline typename ddk::detail::intersection_iterable<Iterables...>::related_iterable operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs);

template<typename IterableValue, typename Allocator, typename ... Iterables>
inline ddk::iteration<typename ddk::detail::intersection_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::intersection_iterable<Iterables...>& i_rhs);

template<typename IterableValue, typename Allocator, typename ... Iterables>
inline ddk::iteration<typename ddk::detail::intersection_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs);

template<typename Traits>
inline ddk::future<void> operator<<=(ddk::attachable<void> i_attachable, ddk::iteration<Traits> i_iteration);

template<typename Traits1, typename Traits2>
inline ddk::detail::union_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>> operator|(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs);

namespace ddk
{
namespace detail
{

template<typename ... Iterables, typename Traits>
inline union_iterable<Iterables...,iterable<Traits>> operator|(const union_iterable<Iterables...>& i_lhs, const iterable<Traits>& i_rhs);

template<typename ... Iterables, typename ... IIterables>
inline union_iterable<Iterables...,typename intersection_iterable<IIterables...>::related_iterable> operator|(const union_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs);

template<typename ... Iterables, typename ... IIterables>
inline union_iterable<typename intersection_iterable<Iterables...>::related_iterable,IIterables...> operator|(const intersection_iterable<IIterables...>& i_lhs, const union_iterable<Iterables...>& i_rhs);

template<typename ... Iterables, typename Traits>
inline intersection_iterable<Iterables...,iterable<Traits>> operator&(const intersection_iterable<Iterables...>& i_lhs, const iterable<Traits>& i_rhs);

template<typename ... Iterables, typename ... IIterables>
inline intersection_iterable<Iterables...,IIterables...> operator&(const intersection_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs);

template<typename ... Iterables, typename ... IIterables>
inline intersection_iterable<Iterables...,typename union_iterable<IIterables...>::related_iterable> operator&(const intersection_iterable<Iterables...>& i_lhs, const union_iterable<IIterables...>& i_rhs);

template<typename ... Iterables, typename ... IIterables>
inline intersection_iterable<typename union_iterable<Iterables...>::related_iterable,IIterables...> operator&(const union_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs);

}
}

template<typename ... Iterables, typename ... IIterables>
inline ddk::detail::union_iterable<Iterables...,IIterables...> operator|(const ddk::detail::union_iterable<Iterables...>& i_lhs, const ddk::detail::union_iterable<IIterables...>& i_rhs);

template<typename Traits1, typename Traits2>
inline ddk::detail::intersection_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>> operator&(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs);


#include "ddk_iterable_utils.inl"
