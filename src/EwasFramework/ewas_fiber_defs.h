#pragma once

#include "type_id.h"
#include "ewas_scoped_enum.h"

namespace ewas
{

struct fiber_impl_t;
typedef Id<size_t,fiber_impl_t> fiber_id;

SCOPED_ENUM_DECL(FiberExecutionState,
	Idle,
	Executing,
	Done);

}