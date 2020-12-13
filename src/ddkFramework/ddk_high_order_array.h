#pragma once

namespace ddk
{
namespace detail
{

template<typename,size_t ...>
class high_order_sub_array;

template<typename T, size_t rank, size_t ... ranks>
class high_order_sub_array<T,rank,ranks...>
{
public:
	high_order_sub_array(T& i_ref);
	high_order_sub_array(const high_order_sub_array& other) = delete;
	high_order_sub_array(high_order_sub_array&& other) = delete;
	high_order_sub_array<T,ranks...> operator[](size_t index);
	high_order_sub_array<const T,ranks...> operator[](size_t index) const;

private:
	T& m_ref;
};

template<typename T>
class high_order_sub_array<T>
{
public:
	high_order_sub_array(T& i_ref);
	high_order_sub_array(const high_order_sub_array& other) = delete;
	high_order_sub_array(high_order_sub_array&& other) = delete;

	template<typename TT>
	high_order_sub_array& operator=(TT&& i_value);
	operator T&();
	operator const T&() const;

private:
	T& m_ref;
};

}

template<typename T, size_t rank, size_t ... ranks>
class high_order_array
{
	static const size_t s_totalSize = mpl::prod_ranks<rank,ranks...>::value;

public:
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;

	high_order_array() = default;
	template<typename ... Args>
	high_order_array(Args&& ... i_initValues);
	high_order_array(const high_order_array<T,rank,ranks...>& other);
	template<typename TT>
	high_order_array(const high_order_array<TT,rank,ranks...>& other);
	~high_order_array() = default;
	detail::high_order_sub_array<T,ranks...> operator[](size_t index);
	detail::high_order_sub_array<const T,ranks...> operator[](size_t index) const;
	high_order_array& operator=(const high_order_array<T,rank,ranks...>& other);
	template<typename TT>
	high_order_array& operator=(const high_order_array<TT,rank,ranks...>& other);
	size_t size() const;

private:
	T m_data[s_totalSize];
};

}

#include "ddk_high_order_array.inl"