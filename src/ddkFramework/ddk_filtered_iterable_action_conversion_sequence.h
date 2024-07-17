//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_action_tags.h"

namespace ddk
{

template<typename ActionTag, typename ... Args>
constexpr ActionTag filtered_iterable_action_cs(Args&& ... i_args);
template<>
constexpr begin_action_tag filtered_iterable_action_cs(const forward_action_tag&);
template<>
constexpr begin_action_tag filtered_iterable_action_cs(forward_action_tag&&);
template<>
constexpr forward_action_tag filtered_iterable_action_cs(const begin_action_tag&);
template<>
constexpr forward_action_tag filtered_iterable_action_cs(begin_action_tag&&);
template<>
constexpr end_action_tag filtered_iterable_action_cs(const backward_action_tag&);
template<>
constexpr end_action_tag filtered_iterable_action_cs(backward_action_tag&&);

}

#include "ddk_filtered_iterable_action_conversion_sequence.inl"