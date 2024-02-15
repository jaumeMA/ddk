#include "ddk_union_iterable_action_conversion_sequence.h"

namespace ddk
{

template<>
begin_action_tag union_iterable_action_cs(const begin_next_iterable&)
{
	return {};
}
template<>
begin_action_tag union_iterable_action_cs(begin_next_iterable&&)
{
	return {};
}
template<>
forward_action_tag union_iterable_action_cs(const begin_next_iterable&)
{
	return {};
}
template<>
forward_action_tag union_iterable_action_cs(begin_next_iterable&&)
{
	return {};
}
template<>
begin_next_iterable union_iterable_action_cs(const begin_action_tag&)
{
	return {};
}
template<>
begin_next_iterable union_iterable_action_cs(begin_action_tag&&)
{
	return {};
}
template<>
begin_next_iterable union_iterable_action_cs(const forward_action_tag&)
{
	return {};
}
template<>
begin_next_iterable union_iterable_action_cs(forward_action_tag&&)
{
	return {};
}
template<>
end_prev_iterable union_iterable_action_cs(const backward_action_tag&)
{
	return {};
}
template<>
end_prev_iterable union_iterable_action_cs(backward_action_tag&&)
{
	return {};
}
template<>
end_prev_iterable union_iterable_action_cs(const end_action_tag&)
{
	return {};
}
template<>
end_prev_iterable union_iterable_action_cs(end_action_tag&&)
{
	return {};
}
template<>
backward_action_tag union_iterable_action_cs(const end_prev_iterable&)
{
	return {};
}
template<>
backward_action_tag union_iterable_action_cs(end_prev_iterable&&)
{
	return {};
}

}