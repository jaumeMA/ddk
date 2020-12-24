#pragma once

#include <utility>
#include "ddk_high_order_array.h"

namespace ddk
{
namespace algo
{

template<typename T, typename TT>
inline decltype(std::declval<T>() + std::declval<TT>()) sum(T&& i_lhs, TT&& i_rhs)
{
	return i_lhs + i_rhs;
}
template<typename Key, typename T, typename TT>
inline std::pair<Key,decltype(std::declval<T>() + std::declval<TT>())> sum(const std::pair<Key,T>& i_lhs, const std::pair<Key,TT>& i_second)
{
	if(i_lhs.first == i_rhs.second)
	{
		return std::make_pair(i_lhs.first,sum(i_lhs.second,i_rhs.second));
	}
	else
	{
		return std::pair<Key,decltype(std::declval<T>() + std::declval<TT>())>();
	}
}
template<typename T, typename TT, size_t ... Indexs>
inline std::pair<const high_order_array<size_t,Indexs...>,decltype(std::declval<T>() + std::declval<TT>())> sum(const std::pair<const high_order_array<size_t,Indexs...>,T>& i_lhs, const std::pair<const high_order_array<size_t,Indexs...>,TT>& i_rhs)
{
	if(i_lhs.first == i_rhs.first)
	{
		return std::make_pair(i_lhs.first,sum(i_lhs.second,i_rhs.second));
	}
	else
	{
		return std::pair<const high_order_array<size_t,Indexs...>,decltype(std::declval<T>() + std::declval<TT>())>();
	}
}

}
}