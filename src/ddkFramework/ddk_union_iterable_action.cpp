#include "ddk_iterable.h"

namespace ddk
{

union_iterable_action<begin_action_tag>::union_iterable_action(begin_action_tag&&)
{
}
begin_action_tag union_iterable_action<begin_action_tag>::action() const
{
	return {};
}

union_iterable_action<last_action_tag>::union_iterable_action(last_action_tag&&)
{
}
last_action_tag union_iterable_action<last_action_tag>::action() const
{
	return {};
}

union_iterable_action<begin_next_iterable>::union_iterable_action(begin_next_iterable&&)
{
}
begin_next_iterable union_iterable_action<begin_next_iterable>::action() const
{
	return {};
}

union_iterable_action<last_prev_iterable>::union_iterable_action(last_prev_iterable&&)
{
}
last_prev_iterable union_iterable_action<last_prev_iterable>::action() const
{
	return {};
}

}