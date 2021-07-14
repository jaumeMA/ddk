#pragma once

#include "ddk_high_order_array.h"
#include "ddk_iterable_concepts.h"
#include "ddk_concepts.h"
#include <vector>

namespace ddk
{
namespace iter
{

class one_to_n_diagonal_action_adapter
{
public:
	template<size_t ... Indexs>
	void initialize(const shift_action& i_initialAction) const;
	template<size_t ... Indexs, typename Action>
	high_order_array<Action,mpl::num_ranks<Indexs...>> resolve(const Action& i_action) const;
};

class one_to_n_enumerate_action_adapter : protected one_to_n_diagonal_action_adapter
{
public:
	TEMPLATE(typename ... Iterables)
	REQUIRES(IS_ITERABLE(Iterables)...)
	one_to_n_enumerate_action_adapter(const Iterables& ... i_iterables);

	template<size_t ... Indexs>
	void initialize(const shift_action& i_initialAction) const;
	template<size_t ... Indexs, typename Action>
	high_order_array<Action,mpl::num_ranks<Indexs...>> resolve(const Action& i_action) const;

private:
	template<typename Action,size_t ... Indexs>
	high_order_array<Action,mpl::num_ranks<Indexs...>> apply_shift(const shift_action& i_action) const;
	template<typename Action, size_t ... Indexs>
	high_order_array<Action,mpl::num_ranks<Indexs...>> get_next_index() const;
	template<typename Action,size_t ... Indexs>
	high_order_array<Action,mpl::num_ranks<Indexs...>> get_prev_index() const;

	const std::vector<size_t> m_maxSize;
	mutable std::vector<size_t> m_currIndexs;
	mutable std::pair<size_t,int> m_currIndex;
};

}
}

#include "ddk_one_to_n_action_adapter.inl"