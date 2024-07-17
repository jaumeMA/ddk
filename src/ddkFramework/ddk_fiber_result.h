//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_result.h"
#include "ddk_async_exceptions.h"

namespace ddk
{

enum class FiberErrorCode
{
	Suspended,
	AsyncExecption,
	InvalidFiberContext
};

typedef error<FiberErrorCode,suspend_exception,async_exception> fiber_error;
typedef result<void,fiber_error> fiber_result;

}