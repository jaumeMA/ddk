
#include "ddk_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
#include "ddk_intersection_iterable_impl.h"
#include "ddk_async.h"

namespace ddk
{

template<typename Iterable, typename IIterable>
Iterable make_iterable(IIterable&& i_iterable)
{
    typedef typename Iterable::traits traits;

    return Iterable{ make_shared_reference<detail::iterable_impl<traits,IIterable>>(std::forward<IIterable>(i_iterable)) };
}

TEMPLATE(typename Iterable)
REQUIRED(IS_ITERABLE(Iterable))
Iterable deduce_iterable(Iterable&& i_iterable)
{
	return i_iterable;
}

TEMPLATE(typename Iterable)
REQUIRED(IS_NOT_ITERABLE(Iterable))
resolved_iterable<Iterable> deduce_iterable(Iterable&& i_iterable)
{
	typedef resolved_iterable<Iterable> iterable_type;
	typedef typename iterable_type::traits traits;

	return iterable_type{ make_shared_reference<detail::iterable_impl<traits,Iterable>>(std::forward<Iterable>(i_iterable)) };
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

namespace ddk
{

template<typename ... Traits>
detail::iterable<detail::union_iterable_traits<Traits...>> concat(const ddk::detail::iterable<Traits>& ... i_iterables)
{
	return ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<Traits>...>>(i_iterables...);
}

template<typename ... Traits>
detail::iterable<detail::intersection_iterable_traits<Traits...>> fusion(const ddk::detail::iterable<Traits>& ... i_iterables)
{
	return ddk::detail::make_iterable_impl<ddk::detail::intersection_iterable_impl<ddk::detail::iterable<Traits> ...>>(i_iterables...);
}

}