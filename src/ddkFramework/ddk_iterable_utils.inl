
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

template<typename Return, typename Type, typename Allocator, template<typename> typename Traits, typename IterableValue>
ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>> operator<<=(const ddk::detail::iterable_transform<ddk::function<Return(Type),Allocator>>& i_lhs, const ddk::detail::iterable<Traits<IterableValue>>& i_rhs)
{
    return ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<Traits<Return>>,Traits<IterableValue>,Return,Type>>(share(i_rhs),i_lhs.get_transform()));
}
template<typename Type, typename Allocator, typename Traits>
ddk::detail::iterable<Traits> operator<<=(const ddk::detail::iterable_filter<ddk::function<bool(Type),Allocator>>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
    return ddk::detail::iterable<Traits>(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<Traits>>(share(i_rhs),i_lhs.get_filter()));
}
template<typename T, typename Traits>
ddk::detail::iterable<Traits> operator<<=(const ddk::detail::iterable_order<T>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
	return ddk::detail::iterable<Traits>(ddk::detail::make_iterable_impl<ddk::detail::ordered_iterable_impl<T,Traits>>(share(i_rhs), i_lhs.get_order()));
}
template<typename IterableValue, typename Allocator, typename Traits>
typename ddk::mpl::static_if<std::is_base_of<ddk::detail::iterable_value_base,IterableValue>::value,ddk::co_iteration<Traits>,ddk::iteration<Traits>>::type operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::iterable<Traits>& i_rhs)
{
	return { i_rhs,i_lhs };
}
template<typename IterableValue, typename Allocator, typename Traits>
typename ddk::mpl::static_if<std::is_base_of<ddk::detail::iterable_value_base,IterableValue>::value,ddk::co_iteration<Traits>,ddk::iteration<Traits>>::type operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
	return { i_rhs,i_lhs };
}
TEMPLATE(typename Return,typename Type,typename Allocator, typename Container)
REQUIRED(IS_NOT_BASE_OF_ITERABLE(Container))
ddk::detail::iterable<ddk::transformed_traits<ddk::resolved_iterable_traits_as<Container,Return>>> operator<<=(const ddk::detail::iterable_transform<ddk::function<Return(Type),Allocator>>& i_lhs, Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;
	typedef ddk::resolved_iterable_traits_as<Container,Return> return_traits_t;

	return ddk::detail::iterable<ddk::transformed_traits<return_traits_t>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<return_traits_t>,traits_t,Return,Type>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_transform()));
}
TEMPLATE(typename Type,typename Allocator,typename Container)
REQUIRED(IS_NOT_BASE_OF_ITERABLE(Container))
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_filter<ddk::function<bool(Type),Allocator>>& i_lhs,Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<traits_t>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_filter()));
}
TEMPLATE(typename T,typename Container)
REQUIRED(IS_NOT_BASE_OF_ITERABLE(Container))
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_order<T>& i_lhs,Container& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::ordered_iterable_impl<T,traits_t>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_order()));
}
TEMPLATE(typename Function,typename Container)
REQUIRED(IS_CALLABLE(Function),IS_NOT_BASE_OF_ITERABLE(Container))
void operator<<=(Function&& i_lhs, Container& i_rhs)
{
	typedef ddk::resolved_iterable_action<const Container> action;

	try
	{
		ddk::visit_iterator(i_rhs,ddk::forwarding_iterable_value_callable<Function,action>{std::forward<Function>(i_lhs)},action{});
	}
	catch(const ddk::suspend_exception&)
	{
	}
}

namespace ddk
{

template<typename ... Traits>
detail::iterable<detail::union_iterable_traits<Traits...>> concat(const ddk::detail::iterable<Traits>& ... i_iterables)
{
    typedef detail::iterable<detail::union_iterable_traits<Traits...>> ret_type;

	return ret_type{ ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<Traits>...>>(i_iterables...) };
}

template<typename ... Traits>
detail::iterable<detail::intersection_iterable_traits<Traits...>> fusion(const ddk::detail::iterable<Traits>& ... i_iterables)
{
    typedef detail::iterable<detail::intersection_iterable_traits<Traits...>> ret_type;

	return ret_type{ ddk::detail::make_iterable_impl<ddk::detail::intersection_iterable_impl<ddk::detail::iterable<Traits> ...>>(i_iterables...) };
}

}
