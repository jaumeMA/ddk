#pragma once

#include "ddk_tuple.h"
#include "ddk_variant.h"

namespace ddk
{

template<typename ... T>
class tuple_adaptor
{
	static const size_t s_numTypes = tuple<T...>::size();

public:
	tuple_adaptor(tuple<T...>& i_iterable,const ddk::iter::shift_action& i_initialAction);
	~tuple_adaptor() = default;
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink) noexcept;
	template<typename Sink>
	inline bool forward_next_value_in(Sink&& i_sink) const noexcept;
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink) noexcept;
	template<typename Sink>
	inline bool forward_prev_value_in(Sink&& i_sink) const noexcept;
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink) noexcept;
	template<typename Sink>
	inline bool forward_shift_value_in(int i_shift,Sink&& i_sink) const noexcept;
	inline bool valid() const noexcept;

private:
	template<typename Sink, size_t ... Indexs>
	inline void get(const mpl::sequence<Indexs...>&, Sink&& i_sink);

	template<typename Sink, size_t ... Indexs>
	inline void get(const mpl::sequence<Indexs...>&,Sink&& i_sink) const;

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink, tuple<T...>&);

	template<size_t Index,typename Sink>
	inline static void _get(Sink&& i_sink,const tuple<T...>&);

	tuple<T...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_tuple_adaptor.inl"