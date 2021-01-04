#pragma once

namespace ddk
{
namespace detail
{

template<typename T,size_t N>
struct counter_tag
{
	friend constexpr size_t counter_index(const counter_tag<T,N>&);
};

template<typename T,size_t N>
struct producer
{
	friend constexpr size_t counter_index(const counter_tag<T,N>&)
	{
		return N;
	}

	static constexpr size_t value = counter_index(counter_tag<T,N>{});
};

template<typename T,size_t N,size_t = producer<T,N>::value>
size_t constexpr consumer(float,const counter_tag<T,N>&,...)
{
	return N;
}

template<typename T,size_t N,size_t V = counter_index(counter_tag<T,N>{}) >
size_t constexpr consumer(int,const counter_tag<T,N>&,size_t R = consumer<T>(0,counter_tag<T,N + 1> {}))
{
	return R;
}

template<typename T,size_t N, size_t V = 0>
size_t constexpr curr_tag(float,const counter_tag<T,N>&,...)
{
	return N - 1;
}

template<typename T,size_t N,size_t V = counter_index(counter_tag<T,N>{}) >
size_t constexpr curr_tag(int,const counter_tag<T,N>&,size_t R = curr_tag<T>(0,counter_tag<T,N + 1> {}))
{
	return R;
}

template<typename T>
size_t constexpr next_value(size_t R = consumer<T>(0,counter_tag<T,0> {}))
{
	return R;
}

template<typename T>
size_t constexpr curr_value(size_t R = curr_tag<T>(0,counter_tag<T,0> {}))
{
	return R;
}

}

template<typename T>
struct static_counter
{
	static constexpr size_t get_next_count(size_t res = detail::next_value<T>())
	{
		return res;
	}
	static constexpr size_t get_curr_count(size_t res = detail::curr_value<T>())
	{
		return res;
	}
};

}