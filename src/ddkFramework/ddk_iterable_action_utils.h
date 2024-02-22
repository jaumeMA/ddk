#pragma once

#include "ddk_iterable_action_concepts.h"
#include "ddk_iterable_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

TEMPLATE(typename Action,typename Iterable)
REQUIRES(IS_ITERABLE_ACTION(Action),IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
inline void operator>>=(Action&& i_action, Iterable&& i_iterable);

}

#include "ddk_iterable_action_utils.inl"