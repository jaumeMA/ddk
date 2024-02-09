#include "ddk_filtered_iterable_action_conversion_sequence.h"

namespace ddk
{

template<>
begin_action_tag filtered_iterable_action_cs(const forward_action_tag&)
{
	return {};
}
template<>
begin_action_tag filtered_iterable_action_cs(forward_action_tag&&)
{
	return {};
}
template<>
forward_action_tag filtered_iterable_action_cs(const begin_action_tag&)
{
	return {};
}
template<>
forward_action_tag filtered_iterable_action_cs(begin_action_tag&&)
{
	return {};
}
template<>
last_action_tag filtered_iterable_action_cs(const backward_action_tag&)
{
	return {};
}
template<>
last_action_tag filtered_iterable_action_cs(backward_action_tag&&)
{
	return {};
}
template<>
backward_action_tag filtered_iterable_action_cs(const last_action_tag&)
{
	return {};
}
template<>
backward_action_tag filtered_iterable_action_cs(last_action_tag&&)
{
	return {};
}

}