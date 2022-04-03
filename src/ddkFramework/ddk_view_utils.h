#pragma once

#include "ddk_iterable_concepts.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_constrained_iterable_impl.h"
#include <array>

namespace ddk
{
namespace view
{

template<typename Type, typename Allocator>
inline detail::iterable_filter<function<bool(Type),Allocator>> filter(const function<bool(Type),Allocator>& i_functor);
TEMPLATE(typename Functor)
REQUIRES(IS_CALLABLE(Functor))
inline detail::iterable_filter<detail::resolved_callable<Functor>> filter(Functor&& i_functor);

template<typename T>
inline detail::iterable_order<T> order(const T& i_order);

TEMPLATE(typename Iterable, typename ... Predicates)
REQUIRES(IS_BASE_OF_ITERABLE(Iterable),IS_CALLABLE(Predicates)...)
inline auto group_by(Iterable&& i_lhs, Predicates&& ... i_predicates);

TEMPLATE(typename Iterable, typename Functor)
REQUIRES(IS_BASE_OF_ITERABLE(Iterable),IS_CALLABLE(Functor))
inline auto constrain(Iterable&& i_rhs, Functor&& i_constrain);
TEMPLATE(typename Functor)
REQUIRES(IS_CALLABLE(Functor))
inline auto constrain(Functor&& i_constrain);
template<typename Iterable>
inline auto take_n(Iterable&& i_rhs, size_t i_numItems);

}
}
