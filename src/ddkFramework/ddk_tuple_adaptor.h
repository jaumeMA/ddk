#pragma once

#include "ddk_tuple.h"
#include "ddk_variant.h"

namespace ddk
{

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
    typedef variant<T...> value_type;

	iterable_adaptor(tuple<T...>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink);
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	template<typename Sink, size_t ... IIndexs>
	inline void get(const mpl::sequence<IIndexs...>&, Sink&& i_sink);

	template<typename Sink, size_t ... IIndexs>
	inline void get(const mpl::sequence<IIndexs...>&,Sink&& i_sink) const;

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink, tuple<T...>&);

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink,const tuple<T...>&);

	tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

template<size_t ... Indexs,typename ... T>
class iterable_adaptor<const detail::tuple_impl<mpl::sequence<Indexs...>,T...>>
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
    typedef variant<T...> value_type;

	iterable_adaptor(const tuple<T...>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink);
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	template<typename Sink, size_t ... IIndexs>
	inline void get(const mpl::sequence<IIndexs...>&, Sink&& i_sink);

	template<typename Sink, size_t ... IIndexs>
	inline void get(const mpl::sequence<IIndexs...>&,Sink&& i_sink) const;

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink, tuple<T...>&);

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink,const tuple<T...>&);

	const tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_tuple_adaptor.inl"
