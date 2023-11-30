#pragma once

#include "ddk_result.h"

namespace ddk
{

enum IterableError
{
    InternalError,
    StopError
};
typedef error<IterableError> iterable_error;
typedef result<void,iterable_error> iterable_result;

}