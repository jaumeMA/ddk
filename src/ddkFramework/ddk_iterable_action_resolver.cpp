#include "ddk_iterable_action_resolver.h"

namespace ddk
{
namespace detail
{

reversable_action_resolver::reversable_action_resolver(bool i_forward)
: m_forward(i_forward)
{
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
alternate_action_resolver alternate_action_resolver::operator()(size_t i_constrain) const
{
	return alternate_action_resolver{ i_constrain,m_forward };
}

//multi_dimensional_action_resolver::multi_dimensional_action_resolver(bool i_forward)
//: reversable_action_resolver(i_forward)
//{
//}
//multi_dimensional_action_resolver::multi_dimensional_action_resolver(bool i_forward, Type i_bottomToTop)
//: reversable_action_resolver(i_forward)
//, m_order(i_bottomToTop)
//{
//}
//multi_dimensional_action_resolver::multi_dimensional_action_resolver(bool i_forward,Type i_bottomToTop,const std::vector<size_t>& i_dimensions)
//: reversable_action_resolver(i_forward)
//, m_order(i_bottomToTop)
//, m_constrains(i_dimensions)
//{
//	m_currDimension = (m_order == Forward) ? 0 : m_constrains.size() - 1;
//	m_currIndex = std::vector<size_t>(m_constrains.size(),0);
//}
//size_t multi_dimensional_action_resolver::get_curr_item() const
//{
//	size_t res = 0;
//	size_t accSize = 1;
//
//	for(size_t index=0;index< m_constrains.size();++index)
//	{
//		res += m_currIndex[index] * accSize;
//		accSize *= m_constrains[index];
//	}
//
//	return res;
//}
//size_t multi_dimensional_action_resolver::get_next_item() const
//{
//	if(m_currIndex[m_currDimension] < m_constrains[m_currDimension] - 1)
//	{
//		m_currIndex[m_currDimension]++;
//	}
//	else
//	{
//		while(m_currIndex[m_currDimension] == m_constrains[m_currDimension] - 1)
//		{
//			m_currIndex[m_currDimension] = 0;
//			m_currDimension--;
//
//			if(m_currDimension == -1)
//			{
//				stop_iteration();
//			}
//		}
//
//		m_currIndex[m_currDimension]++;
//
//		m_currDimension = m_currIndex.size() - 1;
//	}
//
//	return get_curr_item();
//}
//size_t multi_dimensional_action_resolver::get_prev_item() const
//{
//	if(m_currIndex[m_currDimension] > 0)
//	{
//		m_currIndex[m_currDimension]--;
//	}
//	else
//	{
//		while(m_currIndex[m_currDimension] == 0)
//		{
//			m_currDimension++;
//
//			if(m_currDimension == m_constrains.size())
//			{
//				stop_iteration();
//			}
//		}
//
//		m_currIndex[m_currDimension]--;
//
//		m_currDimension = 0;
//	}
//
//	return get_curr_item();
//}

}

const detail::reversable_action_resolver forward_order = detail::reversable_action_resolver(true);
const detail::reversable_action_resolver reverse_order = detail::reversable_action_resolver(false);
//const detail::circular_action_resolver circular_order = detail::reversable_action_resolver(false);
const detail::alternate_action_resolver alternate_order = detail::alternate_action_resolver(true);
//const detail::multi_dimensional_action_resolver dimension_order = detail::multi_dimensional_action_resolver(true,detail::multi_dimensional_action_resolver::Transpose);
//const detail::multi_dimensional_action_resolver transponse_dimension_order = detail::multi_dimensional_action_resolver(true,detail::multi_dimensional_action_resolver::Transpose);

}
