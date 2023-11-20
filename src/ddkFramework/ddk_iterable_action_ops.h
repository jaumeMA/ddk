#pragma once

#include "ddk_iterable_action_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

backward_action inverse(const forward_action&);
forward_action inverse(const backward_action&);
go_to_begin_action inverse(const go_to_end_action&);
go_to_end_action inverse(const go_to_begin_action&);
displacement_action inverse(const displacement_action& i_action);

TEMPLATE(typename Action,typename AAction)
REQUIRES(IS_ITERABLE_ACTION(Action),IS_ITERABLE_ACTION(AAction))
inline auto operator&&(const Action& i_lhs, const AAction& i_rhs);

}

#include "ddk_iterable_action_ops.inl"