//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_static_counter.h"
#include "ddk_rtti.h"

#define ADD_STATIC_TYPE(_TYPE_NAME,_TAG) \
    typename decltype(__get_static_type_list(std::declval<static_typed_number<_TAG,ddk::static_counter<_TAG>::get_curr_count()>>(),std::declval<_TAG>()))::add_unique<_TYPE_NAME>::type __get_static_type_list(const static_typed_number<_TAG,ddk::static_counter<_TAG>::get_next_count()>&,const _TAG&);

namespace ddk
{
namespace mpl
{

template<typename,size_t>
struct static_typed_number
{
};

template<typename Tag>
mpl::type_pack<> __get_static_type_list(const static_typed_number<Tag,0>&, const Tag&);

template<typename Tag>
using static_type_list = decltype(__get_static_type_list(std::declval<static_typed_number<Tag,ddk::static_counter<Tag>::get_curr_count()>>(),std::declval<Tag>()));

}
}