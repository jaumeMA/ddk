
#include "ddk_iterable_impl.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
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
    return ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<Traits<Return>>,Traits<IterableValue>,Return,Type>>(share(i_rhs),i_lhs);
}
template<typename ... Types, typename Allocator, typename Traits>
ddk::detail::iterable<Traits> operator<<=(const ddk::filter_function<bool(Types...),Allocator>& i_lhs, const ddk::detail::iterable<Traits>& i_rhs)
{
    return ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<Traits>>(share(i_rhs),i_lhs);
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
template<typename Traits>
ddk::future<void> operator<<=(ddk::attachable<void> i_attachable, ddk::iteration<Traits> i_iteration)
{
    ddk::shared_reference_wrapper<ddk::async_executor<void>> res = ddk::make_async_executor(ddk::make_function(&ddk::execute_iteration<Traits>,i_iteration));

    res->attach(std::move(i_attachable));

    return res;
}
template<typename Traits1, typename Traits2>
ddk::detail::iterable<ddk::detail::union_iterable_traits<Traits1,Traits2>> operator|(const ddk::detail::iterable<Traits1>& i_lhs, const ddk::detail::iterable<Traits2>& i_rhs)
{
    return ddk::detail::iterable<ddk::detail::union_iterable_traits<Traits1,Traits2>>(ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<Traits1>,ddk::detail::iterable<Traits2>>>(i_lhs,i_rhs));
}
