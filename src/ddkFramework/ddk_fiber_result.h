#pragma once

#include "ddk_result.h"
#include "ddk_async_exceptions.h"

namespace ddk
{

enum class FiberErrorCode
{
	Suspended,
	AsyncExecption
};

typedef error<FiberErrorCode,suspend_exception,async_exception> fiber_error;
typedef result<void,fiber_error> fiber_result;

}