#pragma once

#include "ddk_function.h"
#include "ddk_filter_function.h"
#include "ddk_iteration.h"

namespace ddk
{

template<template<typename> typename Traits, typename IterableType>
using transformed_traits = typename std::enable_if<std::is_same<typename Traits<IterableType>::reference,void>::value==false,typename mpl::static_if<std::is_reference<IterableType>::value,Traits<IterableType>,Traits<typename std::add_const<IterableType>::type>>::type>::type;

template<typename Return, typename ... Types, typename Allocator, template<typename> typename Traits, typename IterableValue>
inline detail::iterable<transformed_traits<Traits,Return>> operator<<=(const function<Return(Types...),Allocator>& i_lhs, const detail::iterable<Traits<IterableValue>>& i_rhs);

template<typename ... Types, typename Allocator, typename Traits>
inline detail::iterable<Traits> operator<<=(const filter_function<bool(Types...),Allocator>& i_lhs, const detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline iteration<Traits> operator<<=(const function<void(IterableValue),Allocator>& i_lhs, detail::iterable<Traits>& i_rhs);

template<typename IterableValue, typename Allocator, typename Traits>
inline iteration<Traits> operator<<=(const function<void(IterableValue),Allocator>& i_lhs, const detail::iterable<Traits>& i_rhs);

template<typename Traits>
inline future<void> operator<<=(attachable<void> i_attachable, iteration<Traits> i_iteration);

}

#include "ddk_iterable_utils.inl"
