
#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename ... Types>
variant<Types...> reversable_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if (i_action.template is<go_forward_action>())
	{
		return (m_reversed) ? action(go_prev_place) : action(go_next_place);
	}
	else if (i_action.template is<go_backward_action>())
	{
		return (m_reversed) ? action(go_next_place) : action(go_prev_place);
	}
	else if (i_action.template is<shift_action>())
	{
		const shift_action& nestedAction = i_action.template get<shift_action>();

		return (m_reversed) ? action(go_to_place(-nestedAction.shifting())) : action(go_to_place(nestedAction.shifting()));
	}
	else
	{
		return i_action;
	}
}

}
}
