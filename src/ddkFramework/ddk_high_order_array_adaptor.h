#pragma once

#include "ddk_high_order_array.h"
#include "ddk_iterable_type.h"

namespace ddk
{

template<typename T, size_t ... ranks>
class iterable_adaptor<high_order_array<T,ranks ...>>
{
	static const size_t s_totalSize = high_order_array<T,ranks ...>::s_totalSize;

public:
	typedef long long difference_type;
	typedef typename high_order_array<T,ranks...>::value_type value_type;
	typedef typename high_order_array<T,ranks...>::reference reference;
	typedef typename high_order_array<T,ranks...>::const_reference const_reference;
	typedef typename high_order_array<T,ranks...>::dimension_t dimension_t;
	typedef mpl::type_pack<begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag> tags_t;

	iterable_adaptor(high_order_array<T,ranks...>& i_iterable);
	auto get_value();
	auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline iterable_action_result<begin_action_tag> perform_action(const begin_action_tag&) const;
	inline iterable_action_result<last_action_tag> perform_action(const last_action_tag&) const;
	inline iterable_action_result<forward_action_tag> perform_action(const forward_action_tag&) const;
	inline iterable_action_result<backward_action_tag> perform_action(const backward_action_tag&) const;
	inline iterable_action_result<displace_action_tag> perform_action(const displace_action_tag&) const;

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	high_order_array<T,ranks ...>& m_iterable;
	mutable difference_type m_currIndex = 0;
};

template<typename T, size_t ... ranks>
class iterable_adaptor<const high_order_array<T,ranks ...>>
{
	static const size_t s_totalSize = high_order_array<T,ranks ...>::s_totalSize;

public:
	typedef long long difference_type;
	typedef typename high_order_array<T,ranks...>::value_type value_type;
	typedef typename high_order_array<T,ranks...>::const_reference reference;
	typedef typename high_order_array<T,ranks...>::const_reference const_reference;
	typedef typename high_order_array<T,ranks...>::dimension_t dimension_t;
	typedef mpl::type_pack<begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag> tags_t;

	iterable_adaptor(const high_order_array<T,ranks...>& i_iterable);
	const_reference get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline iterable_action_result<begin_action_tag> perform_action(const begin_action_tag&) const;
	inline iterable_action_result<last_action_tag> perform_action(const last_action_tag&) const;
	inline iterable_action_result<forward_action_tag> perform_action(const forward_action_tag&) const;
	inline iterable_action_result<backward_action_tag> perform_action(const backward_action_tag&) const;
	inline iterable_action_result<displace_action_tag> perform_action(const displace_action_tag&) const;

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	const high_order_array<T,ranks ...>& m_iterable;
	mutable difference_type m_currIndex = 0;
};

}

#include "ddk_high_order_array_adaptor.inl"