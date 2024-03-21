#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_iterable_action.h"
#include "ddk_variant.h"
#include "ddk_iterable_action_result.h"
#include "ddk_iterable_action_tag_result.h"

namespace ddk
{

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
	typedef detail::iterable_by_type_traits<variant<T...>,
											variant<T&...>,
											variant<const T&...>,
											mpl::type_pack<agnostic_sink_action_tag<variant<T&...>>>,
											mpl::type_pack<agnostic_sink_action_tag<variant<const T&...>>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(tuple<T...>& i_iterable);

	template<typename Adaptor, typename Sink>
	static inline auto perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>&);
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
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

	template<size_t Index, typename Tuple, typename Return>
	inline static Return _agnostic_get(Tuple&);
	template<size_t Index,typename Sink,typename Tuple,typename Return>
	inline static Return _get(const sink_action_tag<Sink>&,Tuple&);

	tuple<T...>& m_iterable;
	mutable typename traits::difference_type m_currIndex = 0;
};

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
	typedef detail::iterable_by_type_traits<variant<T...>,
											variant<const T&...>,
											variant<const T&...>,
											mpl::type_pack<>,
											mpl::type_pack<agnostic_sink_action_tag<variant<const T&...>>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const tuple<T...>& i_iterable);
	template<typename Adaptor, typename Sink>
	static inline auto perform_action(Adaptor&& i_adaptor, const sink_action_tag<Sink>&);
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
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

	template<size_t Index>
	inline static const_reference _agnostic_const_get(const tuple<T...>&);
	template<size_t Index,typename Sink>
	inline static const_reference _const_get(const sink_action_tag<Sink>&,const tuple<T...>&);

	const tuple<T...>& m_iterable;
	mutable typename traits::difference_type m_currIndex = 0;
};

}

#include "ddk_tuple_adaptor.inl"