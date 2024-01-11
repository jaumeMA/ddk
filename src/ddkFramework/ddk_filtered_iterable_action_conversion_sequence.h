#pragma once

#include "ddk_iterable_action_tags.h"

namespace ddk
{

template<typename ActionTag, typename ... Args>
ActionTag filtered_iterable_action_cs(Args&& ... i_args);
template<>
begin_action_tag filtered_iterable_action_cs(const forward_action_tag&);
template<>
forward_action_tag filtered_iterable_action_cs(const begin_action_tag&);
template<>
last_action_tag filtered_iterable_action_cs(const backward_action_tag&);
template<>
backward_action_tag filtered_iterable_action_cs(const last_action_tag&);

}

#include "ddk_filtered_iterable_action_conversion_sequence.inl"