#pragma once

#include "ddk_fiber_result.h"
#include "ddk_exception.h"

namespace ddk
{
namespace detail
{

typedef exception_handler<fiber_result> fiber_exception_handler;

}
}