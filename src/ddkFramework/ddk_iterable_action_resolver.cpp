#include "ddk_iterable_action_resolver.h"

namespace ddk
{
namespace detail
{

reversable_action_resolver::reversable_action_resolver(bool i_forward)
: m_forward(i_forward)
{
}
shift_action reversable_action_resolver::resolve(const shift_action& i_action) const
{
	if(i_action.shifted() == 0)
	{
		return (m_forward) ? go_no_place : go_prev_place;
	}
	else
	{
		return i_action;
	}
}

alternate_action_resolver::alternate_action_resolver(size_t i_constrain, bool i_forward)
: reversable_action_resolver(i_forward)
, m_constrain(i_constrain)
{
}
alternate_action_resolver::alternate_action_resolver(bool i_forward)
: reversable_action_resolver(i_forward)
{
}
shift_action alternate_action_resolver::resolve(const shift_action& i_action) const
{
	if(i_action.shifted() == 0)
	{
		return (m_forward) ? go_no_place : go_prev_place;
	}
	else
	{
		return i_action;
	}
}
alternate_action_resolver alternate_action_resolver::operator()(size_t i_constrain) const
{
	return alternate_action_resolver{ i_constrain,m_forward };
}

}

const detail::reversable_action_resolver forward_order = detail::reversable_action_resolver(true);
const detail::reversable_action_resolver reverse_order = detail::reversable_action_resolver(false);
const detail::alternate_action_resolver alternate_order = detail::alternate_action_resolver(true);

}
