
#include "ddk_iterable_impl.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
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

}

template<typename Return, typename Type, template<typename> typename Traits, typename IterableValue>
ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>> operator<<=(const ddk::function<Return(Type)>& i_lhs, const ddk::detail::iterable<Traits<IterableValue>>& i_rhs)
{
    return ddk::detail::iterable<ddk::transformed_traits<Traits<Return>>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<Traits<Return>>,Traits<IterableValue>,Return,Type>>(share(i_rhs),i_lhs));
}
template<typename ... Types, typename Allocator, typename Traits>
ddk::detail::iterable<Traits> operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
    return ddk::detail::iterable<Traits>(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<Traits>>(share(i_rhs),i_lhs));
}
template<typename IterableValue, typename Allocator, typename Traits>
ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::iterable<Traits>& i_rhs)
{
    return ddk::iteration<Traits>(i_rhs,i_lhs,nullptr);
}
template<typename IterableValue, typename Allocator, typename Traits>
ddk::iteration<Traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
    return ddk::iteration<Traits>(i_rhs,i_lhs,nullptr);
}
template<typename Return, typename Type, typename Allocator, typename ... Iterables>
ddk::detail::iterable<ddk::transformed_traits<ddk::detail::union_iterable_transformed_traits<Return,Iterables...>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::union_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::detail::iterable<ddk::transformed_traits<ddk::detail::union_iterable_transformed_traits<Return,Iterables...>>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<ddk::detail::union_iterable_transformed_traits<Return,Iterables...>>,nested_traits,Return,Type>>(share(i_rhs),as_iterable(i_lhs)));
}
template<typename ... Types, typename Allocator, typename ... Iterables>
typename ddk::detail::union_iterable<Iterables...>::related_iterable operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::union_iterable<Iterables...>::related_iterable related_iterable;
    typedef typename related_iterable::traits nested_traits;

    return related_iterable(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<nested_traits>>(share(i_rhs),as_iterable(i_lhs)));
}
template<typename IterableValue, typename Allocator, typename ... Iterables>
ddk::iteration<typename ddk::detail::union_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::union_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::union_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::iteration<nested_traits>(as_iterable(i_rhs),i_lhs,nullptr);
}
template<typename IterableValue, typename Allocator, typename ... Iterables>
ddk::iteration<typename ddk::detail::union_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::union_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::union_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::iteration<nested_traits>(as_iterable(i_rhs),i_lhs,nullptr);
}
template<typename Return, typename Type, typename Allocator, typename ... Iterables>
ddk::detail::iterable<ddk::transformed_traits<ddk::detail::intersection_iterable_transformed_traits<Return,Iterables...>>> operator<<=(const ddk::function<Return(Type),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::detail::iterable<ddk::transformed_traits<ddk::detail::intersection_iterable_transformed_traits<Return,Iterables...>>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<ddk::detail::intersection_iterable_transformed_traits<Return,Iterables...>>,nested_traits,Return,Type>>(share(i_rhs),as_iterable(i_lhs)));
}
template<typename ... Types, typename Allocator, typename ... Iterables>
typename ddk::detail::intersection_iterable<Iterables...>::related_iterable operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable related_iterable;
    typedef typename related_iterable::traits nested_traits;

    return related_iterable(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<nested_traits>>(share(i_rhs),as_iterable(i_lhs)));
}
template<typename IterableValue, typename Allocator, typename ... Iterables>
ddk::iteration<typename ddk::detail::intersection_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, ddk::detail::intersection_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::iteration<nested_traits>(as_iterable(i_rhs),i_lhs,nullptr);
}
template<typename IterableValue, typename Allocator, typename ... Iterables>
ddk::iteration<typename ddk::detail::intersection_iterable<Iterables...>::related_traits> operator<<=(const ddk::function<void(IterableValue),Allocator>& i_lhs, const ddk::detail::intersection_iterable<Iterables...>& i_rhs)
{
    typedef typename ddk::detail::intersection_iterable<Iterables...>::related_iterable::traits nested_traits;

    return ddk::iteration<nested_traits>(as_iterable(i_rhs),i_lhs,nullptr);
}
template<typename Traits>
ddk::future<void> operator<<=(ddk::attachable<void> i_attachable, ddk::iteration<Traits> i_iteration)
{
    ddk::shared_reference_wrapper<ddk::async_executor<void>> res = ddk::make_async_executor(ddk::make_function(&ddk::execute_iteration<Traits>,i_iteration));

    res->attach(std::move(i_attachable));

    return res;
}
template<typename Traits1, typename Traits2>
ddk::detail::union_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>> operator|(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs)
{
    return ddk::detail::union_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>>(ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>>>(i_lhs,i_rhs));
}

namespace ddk
{
namespace detail
{

template<typename ... Iterables, typename Traits>
union_iterable<Iterables...,iterable<Traits>> operator|(const union_iterable<Iterables...>& i_lhs, const iterable<Traits>& i_rhs)
{
    return union_iterable<Iterables...,iterable<Traits>>(make_shared_reference<union_iterable_impl<Iterables...,iterable<Traits>>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs)));
}
template<typename ... Iterables, typename ... IIterables>
union_iterable<Iterables...,typename intersection_iterable<IIterables...>::related_iterable> operator|(const union_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs)
{
    return union_iterable<Iterables...,typename intersection_iterable<IIterables...>::related_iterable>(make_shared_reference<union_iterable_impl<Iterables...,typename intersection_iterable<IIterables...>::related_iterable>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs.m_iterableImpl)));
}
template<typename ... Iterables, typename ... IIterables>
union_iterable<typename intersection_iterable<Iterables...>::related_iterable,IIterables...> operator|(const intersection_iterable<IIterables...>& i_lhs, const union_iterable<Iterables...>& i_rhs)
{
    return union_iterable<typename intersection_iterable<Iterables...>::related_iterable,IIterables...>(make_shared_reference<union_iterable_impl<typename intersection_iterable<Iterables...>::related_iterable,Iterables...>>(merge_args(i_lhs.m_iterableImpl,i_rhs.m_iterableImpl->get_iterables())));
}
template<typename ... Iterables, typename Traits>
intersection_iterable<Iterables...,iterable<Traits>> operator&(const intersection_iterable<Iterables...>& i_lhs, const iterable<Traits>& i_rhs)
{
    return intersection_iterable<Iterables...,iterable<Traits>>(make_shared_reference<intersection_iterable_impl<Iterables...,iterable<Traits>>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs)));
}
template<typename ... Iterables, typename ... IIterables>
intersection_iterable<Iterables...,IIterables...> operator&(const intersection_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs)
{
    return intersection_iterable<Iterables...,IIterables...>(make_shared_reference<intersection_iterable_impl<Iterables...,IIterables...>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs.m_iterableImpl->get_iterables())));
}
template<typename ... Iterables, typename ... IIterables>
intersection_iterable<Iterables...,typename union_iterable<IIterables...>::related_iterable> operator&(const intersection_iterable<Iterables...>& i_lhs, const union_iterable<IIterables...>& i_rhs)
{
    return intersection_iterable<Iterables...,typename union_iterable<IIterables...>::related_iterable>(make_shared_reference<intersection_iterable_impl<Iterables...,typename union_iterable<IIterables...>::related_iterable>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs.m_iterableImpl)));
}
template<typename ... Iterables, typename ... IIterables>
intersection_iterable<typename union_iterable<Iterables...>::related_iterable,IIterables...> operator&(const union_iterable<Iterables...>& i_lhs, const intersection_iterable<IIterables...>& i_rhs)
{
    return intersection_iterable<typename union_iterable<Iterables...>::related_iterable,IIterables...>(make_shared_reference<intersection_iterable_impl<typename union_iterable<Iterables...>::related_iterable,IIterables...>>(merge_args(i_lhs.m_iterableImpl,i_rhs.m_iterableImpl->get_iterables())));
}

}
}

template<typename ... Iterables, typename ... IIterables>
ddk::detail::union_iterable<Iterables...,IIterables...> operator|(const ddk::detail::union_iterable<Iterables...>& i_lhs, const ddk::detail::union_iterable<IIterables...>& i_rhs)
{
    return ddk::detail::union_iterable<Iterables...,IIterables...>(ddk::make_shared_reference<ddk::detail::union_iterable_impl<Iterables...,IIterables...>>(merge_args(i_lhs.m_iterableImpl->get_iterables(),i_rhs.m_iterableImpl->get_iterables())));
}
template<typename Traits1, typename Traits2>
ddk::detail::intersection_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>> operator&(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs)
{
    return ddk::detail::intersection_iterable<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>>(ddk::detail::make_iterable_impl<ddk::detail::intersection_iterable_impl<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>>>(i_lhs,i_rhs));
}
