#pragma once

#include "ddk_iterable_action_recovery_tags.h"

namespace ddk
{

template<typename,typename>
class filtered_iterable_action;

template<typename Traits,typename Filter>
struct iterable_action_tag_type<Traits,filtered_iterable_action<begin_action_tag,Filter>>
{
	typedef filtered_iterable_action<forward_action_tag,Filter> recovery_type;
	typedef iterable_action_return_type<Traits,forward_action_tag> return_type;
};
template<typename Traits,typename ActionTag,typename Filter>
struct iterable_action_tag_type<Traits,filtered_iterable_action<ActionTag,Filter>>
{
private:
	typedef iterable_action_recovery_tag<Traits,ActionTag> nested_recovery_tag;

public:
	typedef typename mpl::which_type<std::is_same<nested_recovery_tag,void_action_tag>::value,void_action_tag,filtered_iterable_action<nested_recovery_tag,Filter>>::type recovery_type;
	typedef iterable_action_return_type<Traits,ActionTag> return_type;
};

}