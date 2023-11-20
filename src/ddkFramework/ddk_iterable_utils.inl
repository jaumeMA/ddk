
#include "ddk_iteration.h"
#include "ddk_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
#include "ddk_intersection_iterable_impl.h"
#include "ddk_async.h"
#include "ddk_forwarding_iterable_value_callable.h"
#include "ddk_one_to_n_action_adapter.h"
#include "ddk_callable.h"

namespace ddk
{

template<typename Iterable, typename IIterable>
Iterable make_iterable(IIterable&& i_iterable)
{
    typedef typename Iterable::traits traits;

    return Iterable{ make_distributed_reference<detail::iterable_impl<traits,IIterable>>(std::forward<IIterable>(i_iterable)) };
}

TEMPLATE(typename Iterable)
REQUIRED(IS_ITERABLE_TYPE(Iterable))
auto deduce_iterable(Iterable&& i_iterable)
{
	return std::forward<Iterable>(i_iterable);
}

TEMPLATE(typename Container)
REQUIRED(IS_NOT_ITERABLE_TYPE(Container))
resolved_iterable<Container> deduce_iterable(Container&& i_iterable)
{
	typedef typename std::remove_reference<Container>::type container_t;

	return detail::iterable_impl<resolved_iterable_traits<container_t>,container_t>{ std::forward<Container>(i_iterable) };
}

}

template<typename Function, typename Iterable>
auto operator<<=(const ddk::detail::iterable_transform<Function>& i_lhs, Iterable&& i_rhs)
{
	typedef typename ddk::mpl::aqcuire_callable_return_type<Function>::type return_t;
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef typename iterable_t::traits traits;
	typedef typename traits::tags_t::template drop<ddk::add_action_tag<typename traits::value_type>,ddk::remove_action_tag>::type transformed_tags_t;
	typedef ddk::detail::iterable_traits<ddk::detail::iterable_by_type_adaptor<const return_t,transformed_tags_t>> transformed_traits;
	typedef ddk::detail::transformed_iterable_impl<transformed_traits,traits,iterable_t,Function> transformed_iterable;

	return ddk::detail::iterable(transformed_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.get_transform()));
}
template<typename Function,typename Iterable>
auto operator<<=(const ddk::detail::iterable_filter<Function>& i_lhs,Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::filtered_iterable_impl<iterable_t,Function> filtered_iterable;

	return ddk::detail::iterable(filtered_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.get_filter()));
}
template<typename T,typename Iterable>
auto operator<<=(const ddk::detail::iterable_order<T>& i_lhs, Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::ordered_iterable_impl<iterable_t,T> ordered_iterable;

	return ddk::detail::iterable(ordered_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.init(std::forward<Iterable>(i_rhs))));
}
template<typename Function,typename Iterable>
auto operator<<=(const ddk::detail::iterable_constrain<Function>& i_lhs,Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::constrained_iterable_impl<iterable_t,Function> constrained_iterable;

	return ddk::detail::iterable(constrained_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.get_constrain()));
}
TEMPLATE(typename Function, typename Iterable)
REQUIRED(IS_CALLABLE(Function))
auto operator<<=(Function&& i_lhs,Iterable&& i_rhs)
{
	typedef typename ddk::mpl::aqcuire_callable_return_type<Function>::type return_type;

	if constexpr (ddk::mpl::is_void<return_type>)
	{
		return ddk::iteration{ ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),ddk::replaced_return_terse_callable<ddk::forward_action,ddk::mpl::remove_qualifiers<Function>>{ i_lhs,ddk::go_next_place } };
	}
	else
	{
		return ddk::iteration{ ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),std::forward<Function>(i_lhs) };
	}
}

namespace ddk
{

template<typename ... Iterables>
auto concat(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

	return detail::iterable{ detail::union_iterable_impl(deduce_iterable(std::forward<Iterables>(i_iterables)) ...) };
}

template<typename ... Iterables>
auto fusion(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

	return detail::iterable{ detail::intersection_iterable_impl(deduce_iterable(std::forward<Iterables>(i_iterables)) ...)};
}
TEMPLATE(typename ... Iterables)
REQUIRED(IS_ITERABLE(Iterables)...)
detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> enumerate(Iterables&& ... i_iterables)
{
	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");

	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<iter::one_to_n_enumerate_action_adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(iter::one_to_n_enumerate_action_adapter{std::forward<Iterables>(i_iterables)...},deduce_iterable(std::forward<Iterables>(i_iterables))...) };
}
TEMPLATE(typename Adapter,typename ... Iterables)
REQUIRED(IS_ITERABLE(Iterables)...)
detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(Iterables&& ... i_iterables)
{
	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");

	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<Adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(Adapter{},deduce_iterable(std::forward<Iterables>(i_iterables))...) };
}
TEMPLATE(typename Adapter,typename ... Iterables)
REQUIRED(IS_ITERABLE(Iterables)...)
detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(const Adapter& i_adapter,Iterables&& ... i_iterables)
{
	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");

	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<Adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(i_adapter,deduce_iterable(std::forward<Iterables>(i_iterables))...) };
}

namespace iter
{

TEMPLATE(typename T)
REQUIRED(IS_SIZEABLE(T))
size_t size(const T& i_container)
{
	return i_container.size();
}
template<typename T>
size_t size(const const_random_access_iterable<T>& i_iterable)
{
}

template<template<typename,size_t...> typename T,typename R,size_t ... Dims>
std::vector<size_t> dimension(const T<R,Dims...>& i_container)
{
	return { Dims... };
}
TEMPLATE(typename T)
REQUIRED(IS_DIMENSIONABLE(T))
inline auto dimension(const T& i_container)
{
	return i_container.dimension();
}

}
}
