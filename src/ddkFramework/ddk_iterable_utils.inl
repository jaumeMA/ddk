
#include "ddk_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
#include "ddk_intersection_iterable_impl.h"
#include "ddk_async.h"
#include "ddk_forwarding_iterable_value_callable.h"

namespace ddk
{

template<typename Iterable, typename IIterable>
Iterable make_iterable(IIterable&& i_iterable)
{
    typedef typename Iterable::traits traits;

    return Iterable{ make_shared_reference<detail::iterable_impl<traits,IIterable>>(std::forward<IIterable>(i_iterable)) };
}

TEMPLATE(typename Iterable)
REQUIRED(IS_BASE_OF_ITERABLE(Iterable))
Iterable deduce_iterable(Iterable&& i_iterable)
{
	return i_iterable;
}

TEMPLATE(typename Container)
REQUIRED(IS_NOT_BASE_OF_ITERABLE(Container))
resolved_iterable<Container> deduce_iterable(Container&& i_iterable)
{
	typedef resolved_iterable<Container> iterable_type;
	typedef typename iterable_type::traits traits;

	return iterable_type{ make_shared_reference<detail::iterable_impl<traits,Container>>(std::forward<Container>(i_iterable)) };
}

}

template<typename Return,typename Type,typename Allocator, typename Container>
ddk::detail::iterable<ddk::transformed_traits<ddk::resolved_iterable_traits_as<Container,Return>>> operator<<=(const ddk::detail::iterable_transform<ddk::function<Return(Type),Allocator>>& i_lhs, Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;
	typedef ddk::resolved_iterable_traits_as<Container,Return> return_traits_t;

	return ddk::detail::iterable<ddk::transformed_traits<return_traits_t>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<return_traits_t>,traits_t,Return,Type>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_transform()));
}
template<typename Type,typename Allocator,typename Container>
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_filter<ddk::function<bool(Type),Allocator>>& i_lhs,Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<traits_t>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_filter()));
}
template<typename T,typename Container>
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_order<T>& i_lhs,Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::ordered_iterable_impl<T,traits_t>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_order()));
}
TEMPLATE(typename Function, typename Iterable)
REQUIRED(IS_CALLABLE(Function))
auto operator<<=(Function&& i_lhs,Iterable& i_rhs)
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		typedef typename Iterable::traits traits_t;

		typedef typename ddk::mpl::static_if<ddk::concepts::is_iterable_valued_function<typename traits_t::iterable_value,Function>,ddk::co_iteration<Iterable,Function>,ddk::iteration<Iterable,Function>>::type ret_type;

		return ret_type{ i_rhs,std::forward<Function>(i_lhs) };
	}
	else
	{
		return ddk::iteration<Iterable,Function>{ i_rhs,std::forward<Function>(i_lhs) };
	}
}

namespace ddk
{

template<typename ... Iterables>
detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> concat(const Iterables& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

    typedef detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<resolved_iterable_traits<Iterables>>...>>(deduce_iterable(i_iterables) ...) };
}

template<typename ... Iterables>
detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> fusion(const Iterables& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

    typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ ddk::detail::make_iterable_impl<ddk::detail::intersection_iterable_impl<ddk::detail::iterable<resolved_iterable_traits<Iterables>> ...>>(deduce_iterable(i_iterables)...) };
}

}
