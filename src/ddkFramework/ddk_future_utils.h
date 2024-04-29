//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_tuple_values.h"
#include "ddk_array_values.h"

namespace ddk
{

TEMPLATE(typename ... Futures)
REQUIRES(IS_NUMBER_OF_ARGS_GREATER(0,Futures...),TYPE_CONTAINS_SYMBOL(Futures,future_tag)...)
inline auto compose(Futures&& ... i_components);

}