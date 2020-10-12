#pragma once

#include "ddk_intersection_iterable_impl.h"

namespace ddk
{
namespace detail
{

template<typename...>
class union_iterable;

template<typename ... Iterables>
class intersection_iterable
{
    friend inline iterable<intersection_iterable_traits<typename Iterables::traits...>> as_iterable(const intersection_iterable<Iterables...>& i_iterable)
    {
        return iterable<intersection_iterable_traits<typename Iterables::traits...>>(i_iterable.m_iterableImpl);
    }
    template<typename ... IIterables, typename Traits>
    friend inline intersection_iterable<IIterables...,iterable<Traits>> operator&(const intersection_iterable<IIterables...>&, const iterable<Traits>&);
    template<typename ... IIterables, typename ... IIIterables>
    friend inline union_iterable<IIterables...,typename intersection_iterable<IIIterables...>::related_iterable> operator|(const union_iterable<IIterables...>&, const intersection_iterable<IIIterables...>&);
    template<typename ... IIterables, typename ... IIIterables>
    friend inline union_iterable<typename intersection_iterable<IIterables...>::related_iterable,IIIterables...> operator|(const intersection_iterable<IIterables...>& i_lhs, const union_iterable<IIIterables...>&);
    template<typename ... IIterables, typename ... IIIterables>
    friend inline intersection_iterable<IIterables...,typename union_iterable<IIIterables...>::related_iterable> operator&(const intersection_iterable<IIterables...>&, const union_iterable<IIIterables...>&);
    template<typename ... IIterables, typename ... IIIterables>
    friend inline intersection_iterable<typename union_iterable<IIterables...>::related_iterable,IIIterables...> operator&(const union_iterable<IIterables...>&, const intersection_iterable<IIIterables...>&);

public:
    typedef iterable<intersection_iterable_traits<typename Iterables::traits...>> related_iterable;
    typedef typename related_iterable::traits related_traits;

    intersection_iterable(shared_reference_wrapper<intersection_iterable_impl<Iterables...>> i_iterableImpl);

    related_iterable as_iterable() const;
    template<typename Iterable>
    operator Iterable() const;

private:
    shared_reference_wrapper<intersection_iterable_impl<Iterables...>> m_iterableImpl;
};

template<typename>
struct intersection_iterable_traits_adaptor;

template<template<typename>typename Traits, typename IterableValue>
struct intersection_iterable_traits_adaptor<iterable<Traits<IterableValue>>>
{
template<typename TransformedValue>
using transformed_traits = Traits<TransformedValue>;
};

template<typename TransformedValue, typename ... Iterables>
using intersection_iterable_transformed_traits = typename intersection_iterable_traits_adaptor<typename intersection_iterable<Iterables...>::related_iterable>::template transformed_traits<TransformedValue>;

}
}

#include "ddk_intersection_iterable.inl"

