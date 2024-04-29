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

struct begin_next_iterable
{
	constexpr begin_next_iterable() = default;
};

struct end_prev_iterable
{
	constexpr end_prev_iterable() = default;
};

}