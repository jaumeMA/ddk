
#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename Action>
Action reversable_action_resolver::resolve(Action&& i_action) const
{
	if (i_action.is<iter::go_forward_action>())
	{
		return (m_reversed) ? Action(go_prev_place) : Action(go_next_place);
	}
	else if (i_action.is<iter::go_backward_action>())
	{
		return (m_reversed) ? Action(go_next_place) : Action(go_prev_place);
	}
	else if (i_action.is<iter::shift_action>())
	{
		const iter::shift_action& nestedAction = i_action.get<iter::shift_action>();

		return (m_reversed) ? Action(go_shift_place(-nestedAction.shifted())) : Action(go_shift_place(nestedAction.shifted()));
	}
	else
	{
		return i_action;
	}
}

}
}
}