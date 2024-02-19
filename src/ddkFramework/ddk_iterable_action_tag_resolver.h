#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_iterable_action_tags_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename ... T>
static_type_list<mpl::iterable_action_tag> supported_actions(T...);

template<typename,typename>
struct iterable_action_tag_resolver_impl;

template<typename Iterable,typename ... Actions>
struct iterable_action_tag_resolver_impl<Iterable,mpl::type_pack<Actions...>>
{
	typedef mpl::iterable_action_support<Iterable,Actions...> type;
};

template<typename Iterable>
using iterable_action_tag_resolver = typename iterable_action_tag_resolver_impl<Iterable,decltype(supported_actions())>::type;

}
}