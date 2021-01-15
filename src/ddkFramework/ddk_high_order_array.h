#pragma once

#include "ddk_iterable_defs.h"

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
    static inline constexpr size_t size()
    {
        return rank * high_order_sub_array<T,ranks...>::size();
    }

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
    static inline constexpr size_t size()
    {
        return 1;
    }

private:
	T& m_ref;
};

}

template<typename T, size_t rank, size_t ... ranks>
class high_order_array
{
//	DDK_ITERABLE_TYPE(high_order_array,EXPAND_CLASS_TEMPLATE(high_order_array_adaptor,T,rank,ranks...))

    template<typename TT, size_t rrank, size_t ... rranks>
    friend class high_order_array;

public:
	static const size_t s_numRanks = mpl::get_num_ranks<rank,ranks...>();
	static const size_t s_totalSize = mpl::prod_ranks<rank,ranks...>();

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;

	high_order_array() = default;
	high_order_array(const high_order_array<T,rank,ranks...>& other);
	template<typename TT>
	high_order_array(const high_order_array<TT,rank,ranks...>& other);
	~high_order_array() = default;
	detail::high_order_sub_array<T,ranks...> operator[](size_t index);
	detail::high_order_sub_array<const T,ranks...> operator[](size_t index) const;
	reference at(const high_order_array<size_t,s_numRanks>& i_indexs);
	const_reference at(const high_order_array<size_t,s_numRanks>& i_indexs) const;
	reference at(size_t i_index);
	const_reference at(size_t i_index) const;
	high_order_array& operator=(const high_order_array<T,rank,ranks...>& other);
	template<typename TT>
	high_order_array& operator=(const high_order_array<TT,rank,ranks...>& other);
	size_t size() const;
	bool empty() const;

private:
	T m_data[s_totalSize];
};

}

#include "ddk_high_order_array.inl"
#include "ddk_high_order_array_adaptor.h"
