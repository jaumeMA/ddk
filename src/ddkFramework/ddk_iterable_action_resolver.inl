
#include "ddk_iterable_action.h"
#include "ddk_container_concepts.h"
#include "ddk_iterable_utils.h"

namespace ddk
{
namespace detail
{

template<typename Iterable>
reversable_action_resolver reversable_action_resolver::init(Iterable&& i_iterable) const
{
	return *this;
}
template<typename ... Types>
variant<Types...> reversable_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if constexpr (variant<Types...>::template contains<go_forward_action>())
	{
		if (i_action.template is<go_forward_action>())
		{
			return (m_forward) ? action(go_next_place) : action(go_prev_place);
		}
	}
	if constexpr (variant<Types...>::template contains<go_backward_action>())
	{
		if (i_action.template is<go_backward_action>())
		{
			return (m_forward) ? action(go_prev_place) : action(go_next_place);
		}
	}
	if constexpr (variant<Types...>::template contains<shift_action>())
	{
		if (i_action.template is<shift_action>())
		{
			const shift_action& nestedAction = i_action.template get<shift_action>();

			return (m_forward) ? action(go_to_place(nestedAction.shifting())) : action(go_to_place(-nestedAction.shifting()));
		}
	}

	return i_action;
}

template<typename Iterable>
alternate_action_resolver alternate_action_resolver::init(Iterable&& i_iterable) const
{
	if constexpr (IS_SIZEABLE_COND(Iterable))
	{
		return { ddk::iter::size(i_iterable),m_forward };
	}
	else
	{
		return *this;
	}
}
template<typename ... Types>
variant<Types...> alternate_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if constexpr(variant<Types...>::template contains<shift_action>())
	{
		const int currIndex = m_currIndex;
		const bool forward = m_forward;

		if(i_action.template is<go_forward_action>())
		{
			m_currIndex++;
		}
		else if(i_action.template is<go_backward_action>())
		{
			m_currIndex--;
		}
		else if(i_action.template is<shift_action>())
		{
			const shift_action& nestedAction = i_action.template get<shift_action>();

			m_currIndex += static_cast<int>(nestedAction.shifting());
		}

		if(m_currIndex >= 0 && m_currIndex < m_constrain)
		{
			m_forward = !m_forward;

			return action(go_to_place(((forward) ? m_currIndex : -m_currIndex + m_constrain) - currIndex));
		}
		else
		{
			stop_iteration();
		}
	}
	else
	{
		static_assert(sizeof(action)==0,"Alternate orders demmand shiftable iterables.");
	}

	return i_action;
}

TEMPLATE(typename Iterable)
REQUIRED(IS_DIMENSIONABLE(Iterable))
multi_dimensional_action_resolver multi_dimensional_action_resolver::init(Iterable&& i_iterable) const
{
	return multi_dimensional_action_resolver( m_forward,m_order,ddk::iter::dimension(i_iterable) );
}
template<typename ... Types>
variant<Types...> multi_dimensional_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if constexpr(variant<Types...>::template contains<shift_action>())
	{
		if(m_order == Forward)
		{
			return reversable_action_resolver::resolve(i_action);
		}
		else
		{
			if(i_action.template is<go_forward_action>())
			{
				return shift_action(get_next_item(),get_curr_item());
			}
			else if(i_action.template is<go_backward_action>())
			{
				return shift_action(get_prev_item(),get_curr_item());
			}
			else if(i_action.template is<shift_action>())
			{
				const shift_action& nestedAction = i_action.template get<shift_action>();
				const size_t currIndex = get_curr_item();
				const auto shift = nestedAction.shifting();

				for(size_t index=0;index<shift - 1;++index)
				{
					(shift > 0) ? get_next_item() : get_prev_item();
				}

				return shift_action((shift > 0) ? get_next_item() : get_prev_item(),currIndex);
			}
		}
	}
	else
	{
		static_assert(sizeof(action) == 0,"Alternate orders demmand shiftable iterables.");
	}

	return i_action;
}

}
}
