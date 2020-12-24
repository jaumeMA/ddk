#pragma once

#include "ddk_optional.h"
#include <utility>
#include "ddk_high_order_array.h"

namespace ddk
{

template<typename T, size_t ... ranks>
class high_order_array_adaptor
{
public:
	typedef typename high_order_array<T,ranks...>::reference reference;
	typedef typename high_order_array<T,ranks...>::const_reference const_reference;

	high_order_array_adaptor(high_order_array<T,ranks...>& i_iterable,const ddk::iter::shift_action& i_initialAction);
	inline reference get_value() noexcept;
	inline const_reference get_value() const noexcept;
	inline ddk::optional<reference> next_value() noexcept;
	inline ddk::optional<const_reference> next_value() const noexcept;
	inline ddk::optional<reference> prev_value() noexcept;
	inline ddk::optional<const_reference> prev_value() const noexcept;
	inline ddk::optional<reference> shift_value(int i_shift) noexcept;
	inline ddk::optional<const_reference> shift_value(int i_shift) const noexcept;
	inline bool valid() const noexcept;

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>::value;
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	high_order_array<T,ranks ...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_high_order_array_adaptor.inl"