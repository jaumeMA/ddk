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
	typedef detail::iterable_adaptor_traits<high_order_array<T,ranks...>,
											mpl::type_pack<agnostic_sink_action_tag<typename high_order_array<T,ranks...>::reference>>,
											mpl::type_pack<agnostic_sink_action_tag<typename high_order_array<T,ranks...>::const_reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;
	typedef typename high_order_array<T,ranks...>::dimension_t dimension_t;

	iterable_adaptor(high_order_array<T,ranks...>& i_iterable);
	template<typename Adaptor, typename Sink>
	static inline auto perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_sink);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const begin_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const end_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const forward_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const backward_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const displace_action_tag&);

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	high_order_array<T,ranks ...>& m_iterable;
	mutable typename traits::difference_type m_currIndex = 0;
};

template<typename T, size_t ... ranks>
class iterable_adaptor<const high_order_array<T,ranks ...>>
{
	static const size_t s_totalSize = high_order_array<T,ranks ...>::s_totalSize;

public:
	typedef detail::iterable_adaptor_traits<high_order_array<T,ranks...>,
											mpl::empty_type_pack,
											mpl::type_pack<agnostic_sink_action_tag<typename high_order_array<T,ranks...>::const_reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;
	typedef typename high_order_array<T,ranks...>::dimension_t dimension_t;

	iterable_adaptor(const high_order_array<T,ranks...>& i_iterable);
	template<typename Adaptor, typename Sink>
	static inline auto perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>& i_sink);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const begin_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const end_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const forward_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const backward_action_tag&);
	template<typename Adaptor>
	static inline auto perform_action(Adaptor&& i_adaptor, const displace_action_tag&);

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	const high_order_array<T,ranks ...>& m_iterable;
	mutable typename traits::difference_type m_currIndex = 0;
};

}

#include "ddk_high_order_array_adaptor.inl"