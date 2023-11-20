
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
template<typename Action>
auto reversable_action_resolver::operator()(const Action& i_action) const
{
	return inverse(i_action);
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
TEMPLATE(typename ... Actions)
REQUIRED(IS_BASE_OF(shift_action,Actions)...)
variant<Actions...> alternate_action_resolver::resolve(const variant<Actions...>& i_action) const
{
	return visit([this](auto&& ii_action) mutable
	{
		m_currIndex += ii_action.shift();

		if (m_currIndex >= 0 && m_currIndex < m_constrain)
		{
			m_forward = !m_forward;

			Action newAction(i_action);

			newAction.set_shift(((m_forward) ? m_currIndex : -m_currIndex + m_constrain) - currIndex);

			return newAction;
		}
		else
		{
			stop_iteration();
		}
	},i_action);
}

//TEMPLATE(typename Iterable)
//REQUIRED(IS_DIMENSIONABLE(Iterable))
//multi_dimensional_action_resolver multi_dimensional_action_resolver::init(Iterable&& i_iterable) const
//{
//	return multi_dimensional_action_resolver( m_forward,m_order,ddk::iter::dimension(i_iterable) );
//}
//TEMPLATE(typename ... Actions)
//REQUIRED(IS_BASE_OF(shift_action,Actions)...)
//variant<Actions...> multi_dimensional_action_resolver::resolve(const variant<Actions...>& i_action) const
//{
//	typedef variant<Actions...> action;
//
//	if constexpr(variant<Actions...>::template contains<shift_action>())
//	{
//		if(m_order == Forward)
//		{
//			return reversable_action_resolver::resolve(i_action);
//		}
//		else
//		{
//			if(i_action.template is<go_forward_action>())
//			{
//				return shift_action(get_next_item(),get_curr_item());
//			}
//			else if(i_action.template is<go_backward_action>())
//			{
//				return shift_action(get_prev_item(),get_curr_item());
//			}
//			else if(i_action.template is<shift_action>())
//			{
//				const shift_action& nestedAction = i_action.template get<shift_action>();
//				const size_t currIndex = get_curr_item();
//				const auto shift = nestedAction.shifting();
//
//				for(size_t index=0;index<shift - 1;++index)
//				{
//					(shift > 0) ? get_next_item() : get_prev_item();
//				}
//
//				return shift_action((shift > 0) ? get_next_item() : get_prev_item(),currIndex);
//			}
//		}
//	}
//	else
//	{
//		static_assert(sizeof(action) == 0,"Alternate orders demmand shiftable iterables.");
//	}
//
//	return i_action;
//}

}
}
