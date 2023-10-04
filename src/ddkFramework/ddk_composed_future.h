#pragma once

#include "ddk_promise.h"

namespace ddk
{

template<typename>
class composed_future;

template<typename T, unsigned char Dim>
class composed_future<std::array<T,Dim>> : public future<std::array<T, Dim>>
{
	typedef typename ddk::mpl::make_sequence<0, Dim>::type index_seq;

public:
	TEMPLATE(typename ... Futures)
	REQUIRES(IS_BASE_OF(future<T>,Futures)...)
	composed_future(Futures&& ... i_futures);

private:
	struct future_data
	{
		future_data(ddk::promise<std::array<T, Dim>> i_promise);
		~future_data();
		ddk::promise<std::array<T, Dim>> m_promise;
		ddk::atomic_uchar m_resolvedValues = 0;
		ddk::typed_arena<T> m_values[Dim];
	};

	template<unsigned char ... Indexs, typename ... Futures>
	composed_future(const mpl::sequence<Indexs...>& i_seq, ddk::promise<std::array<T, Dim>> i_promise, Futures&& ... i_futures);
	template<unsigned char Index, unsigned char ... Indexs, typename Future>
	inline bool place_future(Future&& i_future, ddk::distributed_reference_wrapper<future_data> i_futureData);
};

template<typename ... T>
class composed_future<std::tuple<T...>> : public future<std::tuple<T...>>
{
	static const unsigned char Dim = mpl::num_types<T...>;
	typedef typename ddk::mpl::make_sequence<0,Dim>::type index_seq;

public:
	TEMPLATE(typename ... Futures)
	REQUIRES(IS_BASE_OF(future<T>, Futures)...)
	composed_future(Futures&& ... i_futures);

private:
	struct future_data
	{
		future_data(ddk::promise<std::tuple<T...>> i_promise);
		~future_data();
		template<unsigned char ... Indexs>
		void destroy_values(const mpl::sequence<Indexs...>& i_seq);

		ddk::promise<std::tuple<T...>> m_promise;
		ddk::atomic_uchar m_resolvedValues = 0;
		ddk::tuple<ddk::typed_arena<T>...> m_values;
	};

	template<unsigned char ... Indexs, typename ... Futures>
	composed_future(const mpl::sequence<Indexs...>& i_seq, ddk::promise<std::tuple<T...>> i_promise, Futures&& ... i_futures);
	template<unsigned char Index, unsigned char ... Indexs, typename Future>
	inline bool place_future(Future&& i_future, ddk::distributed_reference_wrapper<future_data> i_futureData);
};

}