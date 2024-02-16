#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_type_concepts.h"
#include "ddk_iterable_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

typedef mpl::type_pack<begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag,remove_action_tag,mpl::template_class_holder<add_action_tag>,mpl::template_class_holder<agnostic_sink_action_tag>,size_action_tag> supported_actions;

template<typename,typename>
struct iterable_action_tag_resolver_impl;

template<typename Iterable,typename ... Actions>
struct iterable_action_tag_resolver_impl<Iterable,mpl::type_pack<Actions...>>
{
	typedef concepts::iterable_action_support<Iterable,Actions...> type;
};

template<typename Iterable>
using iterable_action_tag_resolver = typename iterable_action_tag_resolver_impl<Iterable,supported_actions>::type;

}
}