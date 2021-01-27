#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_iterable_action_resolver.h"

namespace ddk
{
namespace view
{

template<typename Return, typename Type, typename Allocator>
inline detail::iterable_transform<function<Return(Type),Allocator>> transform(const function<Return(Type),Allocator>& i_functor);
template<typename Functor>
inline detail::iterable_transform<detail::resolved_callable<Functor>> transform(Functor&& i_functor);

template<typename Type, typename Allocator>
inline detail::iterable_filter<function<bool(Type),Allocator>> filter(const function<bool(Type),Allocator>& i_functor);
template<typename Functor>
inline detail::iterable_filter<detail::resolved_callable<Functor>> filter(Functor&& i_functor);

template<typename T>
inline detail::iterable_order<T> order(T&& i_order);

}
}

#include "ddk_view_utils.inl"
