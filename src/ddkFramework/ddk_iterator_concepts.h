//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_static_type_list.h"

#define HAS_ITERATOR_DEFINED_COND(_TYPE) \
    ddk::concepts::has_iterator_defined_v<_TYPE>

#define HAS_ITERATOR_DEFINED(_TYPE) \
    typename std::enable_if<HAS_ITERATOR_DEFINED_COND(_TYPE)>::type

#define IS_CONST_ITERATOR_COND(_TYPE) \
    ddk::concepts::is_const_iterator_defined_v<_TYPE>

#define IS_CONST_ITERATOR(_TYPE) \
    typename std::enable_if<IS_CONST_ITERATOR_COND(_TYPE)>::type

#define IS_NON_CONST_ITERATOR_COND(_TYPE) \
    (ddk::concepts::is_const_iterator_defined_v<_TYPE> == false)

#define IS_NON_CONST_ITERATOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_ITERATOR_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename T>
struct has_iterator_defined
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename TT::iterator*);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool has_iterator_defined_v = has_iterator_defined<mpl::remove_qualifiers<T>>::value;

template<typename T>
inline constexpr bool is_const_iterator_defined_v = mpl::is_const<decltype(std::declval<T>().operator*())>;

}
}
