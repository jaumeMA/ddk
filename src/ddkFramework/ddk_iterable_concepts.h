//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterator_concepts.h"
#include "ddk_container_concepts.h"

#define IS_ITERABLE_TYPE_COND(_TYPE) \
    ddk::concepts::is_iterable_type<ddk::mpl::remove_qualifiers<_TYPE>>::value

#define IS_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_ITERABLE_TYPE_COND(_TYPE)>::type

#define IS_NOT_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_ITERABLE_TYPE_COND(_TYPE) == false>::type

#define IS_CONST_ITERABLE_COND(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE) && (IS_TYPE_CONST_COND(_TYPE) || IS_CONST_ITERATOR_COND(typename _TYPE::iterator))

#define IS_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<IS_CONST_ITERABLE_COND(_TYPE)>::type

#define IS_NON_CONST_ITERABLE_COND(_TYPE) \
    HAS_ITERATOR_DEFINED_COND(_TYPE) && (IS_TYPE_CONST_COND(_TYPE) == false && IS_CONST_ITERATOR_COND(typename _TYPE::iterator) == false)

#define IS_NON_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<IS_NON_CONST_ITERABLE_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename IterableValue,typename T,size_t ... Indexs>
typename mpl::static_if<mpl::holds_type_for_some_type<std::is_constructible,IterableValue&&,typename T::template nth_type<Indexs>...>(),std::true_type,std::false_type>::type _resolve_iterable_valued_function(const mpl::sequence<Indexs...>&);
template<typename Iterable,typename Function,typename T = typename mpl::aqcuire_callable_args_type<Function>::type>
decltype(_resolve_iterable_valued_function<typename Iterable::iterable_value,T>(typename mpl::make_sequence<0,T::size()>::type{})) resolve_iterable_valued_function(const Iterable&,Function&);
template<typename Iterable,typename Function,typename = decltype(std::declval<Function>()(std::declval<typename Iterable::iterable_value>()))>
std::true_type resolve_iterable_valued_function(const Iterable&,const Function&);
template<typename ... T>
std::false_type resolve_iterable_valued_function(const T& ...);

template<typename Iterable,typename Function>
inline constexpr bool is_iterable_valued_function = decltype(resolve_iterable_valued_function(std::declval<Iterable>(),std::declval<mpl::remove_qualifiers<Function>&>()))::value;

template<typename T>
struct is_iterable_type;

template<typename T>
struct is_iterable_type<ddk::detail::iterable<T>>
{
    static const bool value = true;
};
template<typename T>
struct is_iterable_type
{
    static const bool value = false;
};

}
}
