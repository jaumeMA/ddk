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