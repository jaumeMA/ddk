#pragma once

#include "ddk_union_iterable_action_tags.h"

namespace ddk
{

template<typename ActionTag,typename ... Args>
constexpr inline ActionTag union_iterable_action_cs(Args&& ... i_args);
template<typename ActionTag>
constexpr inline ActionTag&& union_iterable_action_cs(ActionTag&&);
template<>
constexpr inline begin_action_tag union_iterable_action_cs(const begin_next_iterable&);
template<>
constexpr inline begin_action_tag union_iterable_action_cs(begin_next_iterable&&);
template<>
constexpr inline forward_action_tag union_iterable_action_cs(const begin_next_iterable&);
template<>
constexpr inline forward_action_tag union_iterable_action_cs(begin_next_iterable&&);
template<>
constexpr inline begin_next_iterable union_iterable_action_cs(const begin_action_tag&);
template<>
constexpr inline begin_next_iterable union_iterable_action_cs(begin_action_tag&&);
template<>
constexpr inline begin_next_iterable union_iterable_action_cs(const forward_action_tag&);
template<>
constexpr inline begin_next_iterable union_iterable_action_cs(forward_action_tag&&);
template<>
constexpr inline end_prev_iterable union_iterable_action_cs(const backward_action_tag&);
template<>
constexpr inline end_prev_iterable union_iterable_action_cs(backward_action_tag&&);
template<>
constexpr inline end_prev_iterable union_iterable_action_cs(const end_action_tag&);
template<>
constexpr inline end_prev_iterable union_iterable_action_cs(end_action_tag&&);
template<>
constexpr inline backward_action_tag union_iterable_action_cs(const end_prev_iterable&);
template<>
constexpr inline backward_action_tag union_iterable_action_cs(end_prev_iterable&&);

}

#include "ddk_union_iterable_action_conversion_sequence.inl"