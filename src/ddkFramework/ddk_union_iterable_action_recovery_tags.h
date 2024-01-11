#pragma once

#include "ddk_union_iterable_action_tags.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename>
class union_iterable_action;

template<typename Iterable>
struct iterable_action_tag_type<Iterable,union_iterable_action<begin_action_tag>>
{
	typedef union_iterable_action<begin_next_iterable> recovery_type;
	typedef iterable_action_return_type<Iterable,begin_action_tag> return_type;
};
template<typename Iterable>
struct iterable_action_tag_type<Iterable,union_iterable_action<forward_action_tag>>
{
	typedef union_iterable_action<begin_next_iterable> recovery_type;
	typedef iterable_action_return_type<Iterable,forward_action_tag> return_type;
};
template<typename Iterable>
struct iterable_action_tag_type<Iterable,union_iterable_action<backward_action_tag>>
{
	typedef union_iterable_action<last_prev_iterable> recovery_type;
	typedef iterable_action_return_type<Iterable,backward_action_tag> return_type;
};
template<typename Iterable>
struct iterable_action_tag_type<Iterable,union_iterable_action<last_action_tag>>
{
	typedef union_iterable_action<last_prev_iterable> recovery_type;
	typedef iterable_action_return_type<Iterable,last_action_tag> return_type;
};

}