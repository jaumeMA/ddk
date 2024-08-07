//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_resolver.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_constrained_iterable_impl.h"
#include "ddk_iterable_concepts.h"
#include "ddk_container_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Iterable>
using resolved_iterable = detail::iterable_correspondence<typename std::remove_reference<Iterable>::type>;
template<typename Iterable>
using resolved_iterable_traits = typename resolved_iterable<Iterable>::traits;
template<typename Iterable, typename T>
using resolved_iterable_traits_as = typename resolved_iterable_traits<Iterable>::template as<T>;

template<typename Traits,typename Iterable, typename Allocator>
inline auto make_iterable(Iterable&& i_iterable, const Allocator& i_allocator);
template<typename Traits,typename Iterable>
inline auto make_iterable(Iterable&& i_iterable);

TEMPLATE(typename Iterable)
REQUIRES(IS_ITERABLE_TYPE(Iterable))
inline Iterable&& deduce_iterable(Iterable&& i_iterable);

}

#include "ddk_iterable_type_concepts.h"

namespace ddk
{

TEMPLATE(typename Container)
REQUIRES(IS_NOT_ITERABLE_TYPE(Container),IS_DEDUCIBLE_ITERABLE_TYPE(Container))
inline resolved_iterable<Container> deduce_iterable(Container&& i_iterable);

template<typename T>
using deduced_iterable = decltype(deduce_iterable(std::declval<T&&>()));

}

TEMPLATE(typename Function, typename Iterable)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline auto operator<<=(const ddk::detail::iterable_transform<Function>& i_lhs, Iterable&& i_rhs);

TEMPLATE(typename Function,typename Iterable)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline auto operator<<=(const ddk::detail::iterable_filter<Function>& i_lhs, Iterable&& i_rhs);

TEMPLATE(typename T,typename Iterable)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline auto operator<<=(const ddk::detail::iterable_order<T>& i_lhs, Iterable&& i_rhs);

TEMPLATE(typename Function,typename Iterable)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline auto operator<<=(const ddk::detail::iterable_constrain<Function>& i_lhs, Iterable&& i_rhs);

TEMPLATE(typename Function,typename Iterable)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline auto operator<<=(const Function& i_lhs, Iterable&&i_rhs);

namespace ddk
{

TEMPLATE(typename ... Iterables)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterables)...)
inline auto concat(Iterables&& ... i_iterables);

TEMPLATE(typename ... Iterables)
REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterables)...)
inline auto fusion(Iterables&& ... i_iterables);

//TEMPLATE(typename ... Iterables)
//REQUIRES(IS_ITERABLE(Iterables)...)
//inline detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> enumerate(Iterables&& ... i_iterables);

//TEMPLATE(typename Adapter,typename ... Iterables)
//REQUIRES(IS_ITERABLE(Iterables)...)
//inline detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(Iterables&& ... i_iterables);
//
//TEMPLATE(typename Adapter,typename ... Iterables)
//REQUIRES(IS_ITERABLE(Iterables)...)
//inline detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(const Adapter& i_adapter, Iterables&& ... i_iterables);

}

#include "ddk_algorithm.h"
