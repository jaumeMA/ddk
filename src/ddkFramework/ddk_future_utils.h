#pragma once

#include "ddk_tuple_values.h"
#include "ddk_vector_values.h"

namespace ddk
{

template<typename T,typename TT>
inline auto operator&&(future<T>&& i_lhs, future<TT>&& i_rhs);
template<typename T, typename TT>
inline auto operator&&(future<T>&& i_lhs,future<values_vector<TT>>&& i_rhs);
template<typename T,typename ... TT>
inline auto operator&&(future<T>&& i_lhs, future<values_tuple<TT...>>&& i_rhs);
template<typename T, typename TT>
inline auto operator&&(future<values_vector<TT>>&& i_lhs,future<T>&& i_rhs);
template<typename T,typename ... TT>
inline auto operator&&(future<values_tuple<TT...>>&& i_lhs, future<T>&& i_rhs);
template<typename T,typename TT>
inline auto operator&&(const shared_future<T>& i_lhs, const shared_future<TT>& i_rhs);
template<typename T,typename TT>
inline auto operator&&(const shared_future<T>& i_lhs, const shared_future<values_vector<TT>>& i_rhs);
template<typename T,typename ... TT>
inline auto operator&&(const shared_future<T>& i_lhs,const shared_future<values_tuple<TT...>>& i_rhs);
template<typename T,typename TT>
inline auto operator&&(const shared_future<values_vector<TT>>& i_lhs, const shared_future<T>& i_rhs);
template<typename T,typename ... TT>
inline auto operator&&(const shared_future<values_tuple<TT...>>& i_lhs, const shared_future<T>& i_rhs);

template<typename ... T>
inline auto fusion(future<T>&& ... i_components);
template<typename ... T>
inline auto fusion(const shared_future<T>& ... i_components);

}