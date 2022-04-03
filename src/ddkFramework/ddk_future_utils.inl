
namespace ddk
{

template<typename T,typename TT>
auto operator&&(future<T>&& i_lhs, future<TT>&& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = std::move(i_rhs)](T i_lhsValue) mutable
	{
		if constexpr (std::is_same<T,TT>::value)
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](TT i_rhsValue) mutable -> values_array<T,2>
			{
				values_array<T,2> res;

				res[0] = std::move(acquiredValue);
				res[1] = std::move(i_rhsValue);

				return res;
			}));
		}
		else
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](TT i_rhsValue) mutable -> values_tuple<T,TT>
			{
				return ddk::make_tuple(std::move(acquiredValue),std::move(i_rhsValue));
			}));
		}
	})));
}
template<typename T, typename TT, size_t Dim>
auto operator&&(future<T>&& i_lhs, future<values_array<TT,Dim>>&& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = std::move(i_rhs)](T i_lhsValue) mutable
	{
		if constexpr (std::is_same<T,TT>::value)
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_array<T,Dim> i_rhsValue) mutable -> values_array<T,Dim+1>
			{
				return i_rhsValue.prepend(std::move(acquiredValue));
			}));
		}
		else
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_array<TT,Dim> i_rhsValue) mutable -> values_tuple<T,values_array<TT,Dim>>
			{
				return ddk::make_tuple(std::move(acquiredValue),std::move(i_rhsValue));
			}));
		}
	})));
}
template<typename T,typename ... TT>
auto operator&&(future<T>&& i_lhs, future<values_tuple<TT...>>&& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = std::move(i_rhs)](T i_lhsValue) mutable
	{
		return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_tuple<TT...> i_rhsValue) mutable -> values_tuple<T,TT...>
		{
			return i_rhsValue.prepend(acquiredValue);
		}));
	})));
}
template<typename T, typename TT, size_t Dim>
auto operator&&(future<values_array<TT,Dim>>&& i_lhs,future<T>&& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = std::move(i_rhs)](values_array<TT,Dim> i_lhsValue) mutable
	{
		if constexpr (std::is_same<T,TT>::value)
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](T i_rhsValue) mutable -> values_array<T,Dim+1>
			{
				return acquiredValue.append(std::move(i_rhsValue));
			}));
		}
		else
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](T i_rhsValue) mutable -> values_tuple<values_array<TT,Dim>,T>
			{
				return ddk::make_tuple(std::move(acquiredValue),std::move(i_rhsValue));
			}));
		}
	})));
}
template<typename T,typename ... TT>
auto operator&&(future<values_tuple<TT...>>&& i_lhs, future<T>&& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = std::move(i_rhs)](values_tuple<TT...> i_lhsValue) mutable
	{
		return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](T i_rhsValue) mutable -> values_tuple<TT...,T>
		{
			return acquiredValue.append(i_rhsValue);
		}));
	})));
}
template<typename T,typename TT>
auto operator&&(const shared_future<T>& i_lhs, const shared_future<TT>& i_rhs)
{
	return i_lhs.then(make_function([acquiredFuture = i_rhs](T i_lhsValue)
	{
		return acquiredFuture.then(make_function([i_lhsValue](TT i_rhsValue)
		{
			return ddk::make_tuple(i_lhsValue,i_rhsValue);
		}));
	}));
}
template<typename T,typename TT, size_t Dim>
auto operator&&(const shared_future<T>& i_lhs,const shared_future<values_array<TT,Dim>>& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = i_rhs](T i_lhsValue) mutable
	{
		if constexpr(std::is_same<T,TT>::value)
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_array<T,Dim> i_rhsValue) mutable -> values_array<T,Dim+1>
			{
				return i_rhsValue.prepend(std::move(acquiredValue));
			}));
		}
		else
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_array<TT,Dim> i_rhsValue) mutable -> values_tuple<T,values_array<TT,Dim>>
			{
				return ddk::make_tuple(std::move(acquiredValue),std::move(i_rhsValue));
			}));
		}
	})));
}
template<typename T,typename ... TT>
auto operator&&(const shared_future<T>& i_lhs,const shared_future<values_tuple<TT...>>& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = i_rhs](T i_lhsValue) mutable
	{
		return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](values_tuple<TT...> i_rhsValue) mutable -> values_tuple<T,TT...>
		{
			return i_rhsValue.prepend(acquiredValue);
		}));
	})));
}
template<typename T,typename TT, size_t Dim>
auto operator&&(const shared_future<values_array<TT,Dim>>& i_lhs,const shared_future<T>& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = i_rhs](values_array<TT,Dim> i_lhsValue) mutable
	{
		if constexpr(std::is_same<T,TT>::value)
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = i_lhsValue](T i_rhsValue) mutable -> values_array<T,Dim+1>
			{
				return acquiredValue.append(std::move(i_rhsValue));
			}));
		}
		else
		{
			return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](T i_rhsValue) mutable -> values_tuple<values_array<TT,Dim>,T>
			{
				return ddk::make_tuple(std::move(acquiredValue),std::move(i_rhsValue));
			}));
		}
	})));
}
template<typename T,typename ... TT>
auto operator&&(const shared_future<values_tuple<TT...>>& i_lhs,const shared_future<T>& i_rhs)
{
	return contraction(std::move(i_lhs).then(make_function([acquiredFuture = i_rhs](values_tuple<TT...> i_lhsValue) mutable
	{
		return std::move(acquiredFuture).then(make_function([acquiredValue = std::move(i_lhsValue)](T i_rhsValue) mutable->values_tuple<TT...,T>
		{
			return acquiredValue.append(i_rhsValue);
		}));
	})));
}
template<typename ... T>
auto fusion(future<T>&& ... i_components)
{
	return (std::move(i_components) && ...);
}
template<typename ... T>
auto fusion(const shared_future<T>& ... i_components)
{
	return (i_components && ...);
}

}
