#include "ddk_iterable_action_resolver.h"

namespace ddk
{
namespace iter
{
namespace detail
{

reversable_action_resolver::reversable_action_resolver(bool i_reversed)
: m_reversed(i_reversed)
{
}
shift_action reversable_action_resolver::resolve(const shift_action& i_action) const
{
	if(i_action.shifted() == 0)
	{
		return (m_reversed) ? go_prev_place : go_no_place;
	}
	else
	{
		return i_action;
	}
}

}

const detail::reversable_action_resolver forward_order = detail::reversable_action_resolver(false);
const detail::reversable_action_resolver reverse_order = detail::reversable_action_resolver(true);

}
}