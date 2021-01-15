#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

class reversable_action_resolver
{
public:
	reversable_action_resolver(bool i_reversed);

	template<typename ... Types>
	variant<Types...> resolve(const variant<Types...>& i_action) const;
	shift_action resolve(const shift_action& i_action) const;

private:
	const bool m_reversed;
};

}

extern const detail::reversable_action_resolver forward_order;
extern const detail::reversable_action_resolver reverse_order;

}

#include "ddk_iterable_action_resolver.inl"
