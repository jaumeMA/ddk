#pragma once

#include "ddk_iterable_defs.h"
#include "ddk_iterable_action_defs.h"
#include "ddk_iterable_type.h"
#include "ddk_variant.h"

namespace ddk
{

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
    typedef variant<T...> value_type;
	typedef variant<T&...> reference;
	typedef variant<const T&...> const_reference;
	typedef long long difference_type;
	typedef mpl::type_pack<begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag> tags_t;

	iterable_adaptor(tuple<T...>& i_iterable);

	inline auto get_value();
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool perform_action(const begin_action_tag&) const;
	inline bool perform_action(const last_action_tag&) const;
	inline bool perform_action(const forward_action_tag&) const;
	inline bool perform_action(const backward_action_tag&) const;
	inline bool perform_action(const displace_action_tag&, difference_type = 0) const;
	inline bool valid() const;

private:
	template<typename Sink, size_t ... IIndexs>
	inline auto get(Sink&& i_sink,const mpl::sequence<IIndexs...>&);
	template<typename Sink, size_t ... IIndexs>
	inline auto get(Sink&& i_sink,const mpl::sequence<IIndexs...>&) const;
	template<size_t Index,typename Sink>
	inline static auto _get(Sink&& i_sink, tuple<T...>&);
	template<size_t Index,typename Sink>
	inline static auto _const_get(Sink&& i_sink,const tuple<T...>&);

	template<size_t ... IIndexs>
	inline auto get(const mpl::sequence<IIndexs...>&);
	template<size_t ... IIndexs>
	inline auto get(const mpl::sequence<IIndexs...>&) const;
	template<size_t Index>
	inline static reference _get(tuple<T...>&);
	template<size_t Index>
	inline static const_reference _const_get(const tuple<T...>&);

	tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
	typedef variant<T...> value_type;
	typedef variant<const T&...> const_reference;
	typedef const_reference reference;
	typedef long long difference_type;
	typedef mpl::type_pack<begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag> tags_t;

	iterable_adaptor(const tuple<T...>& i_iterable);
	inline auto get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool perform_action(const begin_action_tag&) const;
	inline bool perform_action(const last_action_tag&) const;
	inline bool perform_action(const forward_action_tag&) const;
	inline bool perform_action(const backward_action_tag&) const;
	inline bool perform_action(const displace_action_tag&,difference_type) const;
	inline bool valid() const;

private:
	template<typename Sink, size_t ... IIndexs>
	inline void get(const mpl::sequence<IIndexs...>&,Sink&& i_sink) const;
	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink,const tuple<T...>&);

	template<size_t ... IIndexs>
	inline auto get(const mpl::sequence<IIndexs...>&) const;
	template<size_t Index>
	inline static const_reference _get(const tuple<T...>&);

	const tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_tuple_adaptor.inl"