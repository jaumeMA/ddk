//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_static_counter.h"

#define ADD_STATIC_TYPE(_TYPE_NAME,_TAG) \
    friend inline typename decltype(__get_static_type_list(std::declval<ddk::detail::static_typed_number<_TAG,ddk::static_counter<_TAG>::get_curr_count()>>()))::add_unique<_TYPE_NAME>::type __get_static_type_list(const ddk::detail::static_typed_number<_TAG,ddk::static_counter<_TAG>::get_next_count()>&); \

namespace ddk
{
namespace detail
{

template<typename Tag,size_t Number>
struct static_typed_number
{
};
template<typename T>
struct static_typed_number<T,0>
{
    friend inline mpl::type_pack<> __get_static_type_list(const static_typed_number&);
};

template<typename Tag>
using static_type_list = decltype(__get_static_type_list(std::declval<static_typed_number<Tag,ddk::static_counter<Tag>::get_curr_count()>>()));

}
}