#pragma once

#include "ddk_iterable_result.h"

namespace ddk
{

template<typename Action>
using iterable_action_result = result<Action,iterable_error>;

}

#include "ddk_iterable_action_result.inl"