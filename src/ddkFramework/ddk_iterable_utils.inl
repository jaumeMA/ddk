
#include "ddk_iteration.h"
#include "ddk_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
#include "ddk_intersection_iterable_impl.h"
#include "ddk_async.h"
#include "ddk_forwarding_iterable_value_callable.h"
#include "ddk_one_to_n_action_adapter.h"
#include "ddk_callable.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Traits,typename Iterable,typename Allocator>
auto make_iterable(Iterable&& i_iterable,const Allocator& i_allocator)
{
	typedef mpl::remove_qualifiers<Iterable> iterable_t;
	typedef typename iterable_t::traits from_traits;
	typedef Traits to_traits;

	if constexpr (mpl::is_same_type<from_traits,to_traits>::value)
	{
		return make_distributed_reference<detail::view_iterable<iterable_t>>(i_allocator,std::forward<Iterable>(i_iterable));
	}
	else
	{
		typedef detail::transformed_iterable_impl<to_traits,from_traits,detail::iterable<iterable_t>,detail::traits_conversion_callable<from_traits,to_traits>> transformed_iterable;

		return make_distributed_reference<detail::view_iterable<transformed_iterable>>(i_allocator,detail::view_iterable(std::forward<Iterable>(i_iterable)),detail::traits_conversion_callable<from_traits,to_traits>());
	}
}
template<typename Traits,typename Iterable>
auto make_iterable(Iterable&& i_iterable)
{
	return make_iterable<Traits>(std::forward<Iterable>(i_iterable),g_system_allocator);
}

TEMPLATE(typename Iterable)
REQUIRED(IS_ITERABLE_TYPE(Iterable))
Iterable&& deduce_iterable(Iterable&& i_iterable)
{
	return std::forward<Iterable>(i_iterable);
}

TEMPLATE(typename Container)
REQUIRED(IS_NOT_ITERABLE_TYPE(Container),IS_DEDUCIBLE_ITERABLE_TYPE(Container))
resolved_iterable<Container> deduce_iterable(Container&& i_iterable)
{
	return { std::forward<Container>(i_iterable) };
}

}

TEMPLATE(typename Function,typename Iterable)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
auto operator<<=(const ddk::detail::iterable_transform<Function>& i_lhs, Iterable&& i_rhs)
{
	typedef typename ddk::mpl::aqcuire_callable_return_type<Function>::type return_t;
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef typename iterable_t::traits traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	typedef typename tags_t::template drop_if<ddk::mpl::is_not_type<ddk::agnostic_sink_action_tag<typename traits::reference>>::template type>::type simplified_tags;
	typedef typename ddk::mpl::action_tags_retrait<traits,ddk::detail::by_value_traits<return_t>,ddk::detail::reduce_type_traits,simplified_tags>::type transformed_tags;
	typedef typename ddk::mpl::action_tags_retrait<traits,ddk::detail::by_value_traits<return_t>,ddk::detail::reduce_type_traits,const_tags_t>::type transformed_const_tags;

	typedef ddk::detail::iterable_traits<ddk::detail::iterable_by_value_adaptor<return_t,transformed_tags,transformed_const_tags>> iterable_transformed_traits;
	typedef ddk::detail::transformed_iterable_impl<iterable_transformed_traits,traits,iterable_t,Function> transformed_iterable;

	return ddk::detail::iterable(transformed_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs));
}
TEMPLATE(typename Function,typename Iterable)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
auto operator<<=(const ddk::detail::iterable_filter<Function>& i_lhs,Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::filtered_iterable_impl<iterable_t,Function> filtered_iterable;

	return ddk::detail::iterable(filtered_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.get_filter()));
}
TEMPLATE(typename T,typename Iterable)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
auto operator<<=(const ddk::detail::iterable_order<T>& i_lhs, Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::ordered_iterable_impl<iterable_t,T> ordered_iterable;

	return ddk::detail::iterable(ordered_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.order()));
}
TEMPLATE(typename Function,typename Iterable)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
auto operator<<=(const ddk::detail::iterable_constrain<Function>& i_lhs,Iterable&& i_rhs)
{
	typedef ddk::resolved_iterable<Iterable> iterable_t;
	typedef ddk::detail::constrained_iterable_impl<iterable_t,Function> constrained_iterable;

	return ddk::detail::iterable(constrained_iterable(ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs.get_constrain()));
}
TEMPLATE(typename Function,typename Iterable)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
auto operator<<=(const Function& i_lhs,Iterable&& i_rhs)
{
	return ddk::iteration{ ddk::deduce_iterable(std::forward<Iterable>(i_rhs)),i_lhs };
}

namespace ddk
{

TEMPLATE(typename ... Iterables)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterables)...)
auto concat(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

	return detail::iterable{ detail::union_iterable_impl(deduce_iterable(std::forward<Iterables>(i_iterables)) ...) };
}

TEMPLATE(typename ... Iterables)
REQUIRED(IS_DEDUCIBLE_ITERABLE_TYPE(Iterables)...)
auto fusion(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

	return detail::iterable{ detail::intersection_iterable_impl(deduce_iterable(std::forward<Iterables>(i_iterables)) ...)};
}
//TEMPLATE(typename ... Iterables)
//REQUIRED(IS_ITERABLE(Iterables)...)
//detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> enumerate(Iterables&& ... i_iterables)
//{
//	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");
//
//	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;
//
//	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<iter::one_to_n_enumerate_action_adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(iter::one_to_n_enumerate_action_adapter(std::forward<Iterables>(i_iterables)...),deduce_iterable(std::forward<Iterables>(i_iterables))...) };
//}
//TEMPLATE(typename Adapter,typename ... Iterables)
//REQUIRED(IS_ITERABLE(Iterables)...)
//detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(Iterables&& ... i_iterables)
//{
//	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");
//
//	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;
//
//	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<Adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(Adapter{},deduce_iterable(std::forward<Iterables>(i_iterables))...) };
//}
//TEMPLATE(typename Adapter,typename ... Iterables)
//REQUIRED(IS_ITERABLE(Iterables)...)
//detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> combine(const Adapter& i_adapter,Iterables&& ... i_iterables)
//{
//	static_assert(mpl::num_types<Iterables...> != 0,"You shall provider more than 0 iterables");
//
//	typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;
//
//	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<Adapter,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(i_adapter,deduce_iterable(std::forward<Iterables>(i_iterables))...) };
//}

}
