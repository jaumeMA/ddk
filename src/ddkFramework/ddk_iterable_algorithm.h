#pragma once

#include "ddk_iteration.h"
#include "ddk_iterable_value.h"
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_iterable_utils.h"
#include "ddk_tuple.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename ... Iterables>
class iterable_pack
{
public:
	explicit iterable_pack(const Iterables& ... i_iterables);

	template<size_t Index>
	inline const typename mpl::nth_type_of<Index,Iterables...>::type& get() const;

private:
	tuple<const Iterables...> m_iterables;
};

}


template<typename ... Iterables>
inline detail::iterable_pack<decltype(deduce_iterable(std::declval<const Iterables&>()))...> iterable_sum(const Iterables& ... i_iterables);

}
}

template<typename Sink,typename ... Iterables>
inline ddk::future<ddk::iter::action_result> operator<<=(Sink&& i_lhs,const ddk::trans::detail::iterable_pack<Iterables...>& i_rhs);


#include "ddk_iterable_algorithm.inl"