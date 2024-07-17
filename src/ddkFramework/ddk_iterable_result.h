//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_result.h"

namespace ddk
{

enum class IterableError
{
    InternalError,
    UserError
};
typedef error<IterableError> iterable_error;
typedef result<void,iterable_error> iterable_result;

}