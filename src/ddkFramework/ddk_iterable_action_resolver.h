#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Traits)
REQUIRES(ACTION_TAGS_SUPPORTED(const Traits,begin_action_tag,forward_action_tag))
go_to_begin_action _resolve_default_action(const Traits&);

TEMPLATE(typename Traits)
REQUIRES(ACTION_TAGS_NOT_SUPPORTED(const Traits,begin_action_tag,forward_action_tag),ACTION_TAGS_SUPPORTED(const Traits,end_action_tag,backward_action_tag))
go_to_end_action _resolve_default_action(const Traits&,...);

template<typename Traits>
struct default_action_deduced_by_traits
{
	typedef decltype(_resolve_default_action(std::declval<Traits>())) type;

	static constexpr auto default_action()
	{
		return type{};
	}
};

template<typename Traits>
struct default_action_provided_by_traits
{
	typedef decltype(Traits::default_action()) type;

	static constexpr auto default_action()
	{
		return Traits::default_action();
	}
};

template<typename Traits>
default_action_provided_by_traits<Traits> resolve_default_action(Traits&&,decltype(Traits::default_action())*);

template<typename Traits>
default_action_deduced_by_traits<Traits> resolve_default_action(Traits&&,...);

}

template<typename Iterable>
using iterable_default_action = decltype(detail::resolve_default_action(std::declval<typename Iterable::traits>(),nullptr));

}