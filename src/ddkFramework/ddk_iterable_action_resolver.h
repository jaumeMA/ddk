#pragma once

namespace ddk
{
namespace iter
{
namespace detail
{

class reversable_action_resolver
{
public:
	reversable_action_resolver(bool i_reversed);

	template<typename Action>
	Action resolve(Action&& i_action) const;

private:
	const bool m_reversed;
};

}

extern const detail::reversable_action_resolver forward_order;
extern const detail::reversable_action_resolver reverse_order;

}
}

#include "ddk_iterable_action_resolver.inl"