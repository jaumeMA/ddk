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