//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_union_iterable_action_tags.h"

namespace ddk
{

template<typename>
class union_iterable_action;

template<typename Iterable>
struct iterable_action_tag_type<Iterable,begin_next_iterable>
{
	typedef begin_next_iterable recovery_type;
	typedef iterable_action_return_type<Iterable,begin_action_tag> return_type;
};
template<typename Iterable>
struct iterable_action_tag_type<Iterable,end_prev_iterable>
{
	typedef end_prev_iterable recovery_type;
	typedef iterable_action_return_type<Iterable,backward_action_tag> return_type;
};

}