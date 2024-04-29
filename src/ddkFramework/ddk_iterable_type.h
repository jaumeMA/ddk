//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename T>
using forward_iterable_action_tag = mpl::type_pack<add_action_tag<T>,remove_action_tag,forward_action_tag>;
typedef mpl::type_pack<forward_action_tag> const_forward_iterable_action_tag;
template<typename T>
using backward_iterable_action_tag = mpl::type_pack<add_action_tag<T>,remove_action_tag,backward_action_tag>;
typedef mpl::type_pack<backward_action_tag> const_backward_iterable_action_tag;
template<typename T>
using bidirectional_iterable_action_tag = mpl::type_pack<add_action_tag<T>,remove_action_tag,forward_action_tag,backward_action_tag>;
typedef mpl::type_pack<forward_action_tag,backward_action_tag> const_bidirectional_iterable_action_tag;
template<typename T>
using random_access_iterable_action_tag = mpl::type_pack<add_action_tag<T>,remove_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>;
typedef mpl::type_pack<forward_action_tag,backward_action_tag,displace_action_tag> const_random_access_iterable_action_tag;

}
}
