#pragma once

#include "ddk_union_iterable_action_tags.h"

namespace ddk
{

template<typename ActionTag,typename ... Args>
ActionTag union_iterable_action_cs(Args&& ... i_args);
template<>
begin_action_tag union_iterable_action_cs(const begin_next_iterable&);
template<>
begin_action_tag union_iterable_action_cs(begin_next_iterable&&);
template<>
forward_action_tag union_iterable_action_cs(const begin_next_iterable&);
template<>
forward_action_tag union_iterable_action_cs(begin_next_iterable&&);
template<>
begin_next_iterable union_iterable_action_cs(const begin_action_tag&);
template<>
begin_next_iterable union_iterable_action_cs(begin_action_tag&&);
template<>
begin_next_iterable union_iterable_action_cs(const forward_action_tag&);
template<>
begin_next_iterable union_iterable_action_cs(forward_action_tag&&);
template<>
end_prev_iterable union_iterable_action_cs(const backward_action_tag&);
template<>
end_prev_iterable union_iterable_action_cs(backward_action_tag&&);
template<>
end_prev_iterable union_iterable_action_cs(const end_action_tag&);
template<>
end_prev_iterable union_iterable_action_cs(end_action_tag&&);
template<>
backward_action_tag union_iterable_action_cs(const end_prev_iterable&);
template<>
backward_action_tag union_iterable_action_cs(end_prev_iterable&&);

}

#include "ddk_union_iterable_action_conversion_sequence.inl"