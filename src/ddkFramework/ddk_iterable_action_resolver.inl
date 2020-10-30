
#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{
namespace detail
{

template<typename ... Types>
variant<Types...> reversable_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if (i_action.is<iter::go_forward_action>())
	{
		return (m_reversed) ? action(go_prev_place) : action(go_next_place);
	}
	else if (i_action.is<iter::go_backward_action>())
	{
		return (m_reversed) ? action(go_next_place) : action(go_prev_place);
	}
	else if (i_action.is<iter::shift_action>())
	{
		const iter::shift_action& nestedAction = i_action.get<iter::shift_action>();

		return (m_reversed) ? action(go_to_place(-nestedAction.shifted())) : action(go_to_place(nestedAction.shifted()));
	}
	else
	{
		return i_action;
	}
}

}
}
}