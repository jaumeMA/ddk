//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define IS_DEDUCIBLE_ITERABLE_TYPE_COND(_TYPE) \
    ddk::concepts::is_iterable_deducible_type<ddk::mpl::remove_qualifiers<_TYPE>>

#define IS_DEDUCIBLE_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_DEDUCIBLE_ITERABLE_TYPE_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename T>
inline constexpr bool is_iterable_deducible_type = (ddk::iterable_adaptor<T>::tags_t::empty() == false ||
                                                    ddk::iterable_adaptor<T>::const_tags_t::empty() == false);

}
}