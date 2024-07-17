//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_constrained_iterable_impl.h"
#include "ddk_iterable_concepts.h"
#include "ddk_concepts.h"
#include <array>

namespace ddk
{
namespace view
{

TEMPLATE(typename Functor)
REQUIRES(IS_CALLABLE(Functor))
inline ddk::detail::iterable_filter<Functor> filter(Functor&& i_functor);

template<typename T>
inline ddk::detail::iterable_order<T> order(const T& i_order);

TEMPLATE(typename Iterable, typename ... Predicates)
REQUIRES(IS_ITERABLE_TYPE(Iterable),IS_CALLABLE(Predicates)...)
inline auto group_by(Iterable&& i_lhs, Predicates&& ... i_predicates);

TEMPLATE(typename Functor)
REQUIRES(IS_CALLABLE(Functor))
inline ddk::detail::iterable_constrain<Functor> constrain(Functor&& i_constrain);

template<typename Iterable>
inline auto take_n(Iterable&& i_rhs, size_t i_numItems);

}
}