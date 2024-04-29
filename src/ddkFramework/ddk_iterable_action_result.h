//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_result.h"

namespace ddk
{

template<typename Action>
using iterable_action_result = result<Action,iterable_error>;

}

#include "ddk_iterable_action_result.inl"