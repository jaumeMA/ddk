
#include <utility>

namespace ddk
{
namespace iter
{

//template<size_t ... Indexs>
//void one_to_n_diagonal_action_adapter::initialize(const shift_action& i_initialAction) const
//{
//	//do nothing
//}
//template<size_t ... Indexs, typename Action>
//high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_diagonal_action_adapter::resolve(const Action& i_action) const
//{
//	high_order_array<Action,mpl::num_ranks<Indexs...>> res;
//
//	for(size_t index = 0;index<mpl::num_ranks<Indexs...>;++index)
//	{
//		res[index] = i_action;
//	}
//
//	return res;
//}
//
//TEMPLATE(typename ... Iterables)
//REQUIRED(IS_ITERABLE(Iterables)...)
//one_to_n_enumerate_action_adapter::one_to_n_enumerate_action_adapter(const Iterables& ... i_iterables)
//: m_maxSize{ iter::size(i_iterables)... }
//, m_currIndex({0,0})
//{
//	constexpr size_t k_numTypes = mpl::num_types<Iterables...>;
//
//	m_currIndexs.reserve(k_numTypes);
//	for(size_t index = 0; index < k_numTypes; ++index)
//	{
//		m_currIndexs[index] = 0;
//	}
//}
//template<size_t ... Indexs>
//void one_to_n_enumerate_action_adapter::initialize(const shift_action& i_initialAction) const
//{
//	//normalize negative values
//	auto shift = i_initialAction.shifting();
//
//	if(shift < 0)
//	{
//		size_t accSize = 1;
//		for(size_t index = 0; index < m_maxSize.size(); ++index)
//		{
//			accSize *= m_maxSize[index];
//		}
//
//		apply_shift<shift_action,Indexs...>(shift_action(accSize + shift));
//	}
//	else
//	{
//		apply_shift<shift_action,Indexs...>(i_initialAction);
//	}
//}
//template<size_t ... Indexs, typename Action>
//high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_enumerate_action_adapter::resolve(const Action& i_action) const
//{
//	if(i_action.template is_base_of<shift_action>())
//	{
//		return apply_shift<Action,Indexs...>(i_action.template get_as<shift_action>());
//	}
//	else
//	{
//		return one_to_n_diagonal_action_adapter::resolve<Indexs...>(i_action);
//	}
//}
//template<typename Action,size_t ... Indexs>
//high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_enumerate_action_adapter::apply_shift(const shift_action& i_action) const
//{
//	const auto shift = i_action.shifting();
//
//	if(shift == 0)
//	{
//		return { mpl::index_to_type<Indexs,Action>(go_no_place) ... };
//	}
//	else
//	{
//		for(size_t index = 0; index < std::abs(shift) - 1; ++index)
//		{
//			(shift > 0) ? get_next_index<Action,Indexs...>() : get_prev_index<Action,Indexs...>();
//		}
//
//		return (shift > 0) ? get_next_index<Action,Indexs...>() : get_prev_index<Action,Indexs...>();
//	}
//}
//template<typename Action, size_t ... Indexs>
//high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_enumerate_action_adapter::get_next_index() const
//{
//	high_order_array<Action,mpl::num_ranks<Indexs...>> res;
//
//	if(m_currIndex.second + 1 < m_maxSize[m_currIndex.first])
//	{
//		m_currIndex.second++;
//
//		for(size_t index = 0; index < mpl::num_ranks<Indexs...>; ++index)
//		{
//			res[index] = (index == m_currIndex.first) ? go_next_place : go_to_place(0);
//		}
//	}
//	else
//	{
//		for(size_t index = 0; index < mpl::num_ranks<Indexs...>; ++index)
//		{
//			res[index] = (index == (m_currIndex.first + 1)) ? go_next_place : (index > (m_currIndex.first + 1)) ? go_no_place :  go_to_place(0,m_maxSize[index] - 1);
//		}
//
//		m_currIndex = std::make_pair(0,0);
//	}
//
//	return res;
//}
//template<typename Action,size_t ... Indexs>
//high_order_array<Action,mpl::num_ranks<Indexs...>> one_to_n_enumerate_action_adapter::get_prev_index() const
//{
//	high_order_array<Action,mpl::num_ranks<Indexs...>> res;
//
//	if(m_currIndex.second - 1 >= 0)
//	{
//		m_currIndex.second--;
//
//		for(size_t index = 0; index < mpl::num_ranks<Indexs...>; ++index)
//		{
//			res[index] = (index == m_currIndex.first) ? go_prev_place : go_to_place(0);
//		}
//	}
//	else
//	{
//		for(size_t index = 0; index < mpl::num_ranks<Indexs...>; ++index)
//		{
//			res[index] = (index == (m_currIndex.first + 1)) ? go_prev_place : (index > (m_currIndex.first + 1)) ? go_no_place : go_to_place(m_maxSize[index] - 1,0);
//		}
//
//		m_currIndex = std::make_pair(0,m_maxSize[0] - 1);
//	}
//
//	return res;
//}

}
}