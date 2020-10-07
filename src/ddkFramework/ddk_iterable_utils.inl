
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
template<typename Return, typename ... Types, template<typename> typename Traits, typename IterableValue>
detail::iterable<transformed_traits<Traits,Return>> operator<<=(const function<Return(Types...)>& i_lhs, const detail::iterable<Traits<IterableValue>>& i_rhs)
{
    return detail::make_iterable_impl<detail::transformed_iterable_impl<transformed_traits<Traits,Return>,Traits<IterableValue>>>(share(i_rhs),i_lhs);
}
template<typename ... Types, typename Allocator, typename Traits>
detail::iterable<Traits> operator<<=(const filter_function<bool(Types...),Allocator>& i_lhs, const detail::iterable<Traits>& i_rhs)
{
    return detail::make_iterable_impl<detail::filtered_iterable_impl<Traits>>(share(i_rhs),i_lhs);
}
template<typename IterableValue, typename Allocator, typename Traits>
iteration<Traits> operator<<=(const function<void(IterableValue),Allocator>& i_lhs, detail::iterable<Traits>& i_rhs)
{
    return iteration<Traits>(i_rhs,i_lhs,nullptr);
}
template<typename IterableValue, typename Allocator, typename Traits>
iteration<Traits> operator<<=(const function<void(IterableValue),Allocator>& i_lhs, const detail::iterable<Traits>& i_rhs)
{
    return iteration<Traits>(i_rhs,i_lhs,nullptr);
}
template<typename Traits>
future<void> operator<<=(attachable<void> i_attachable, iteration<Traits> i_iteration)
{
    shared_reference_wrapper<async_executor<void>> res = make_async_executor(make_function(&ddk::execute_iteration<Traits>,i_iteration));

    res->attach(std::move(i_attachable));

    return res;
}
template<typename Traits1, typename Traits2>
detail::iterable<detail::union_iterable_traits<Traits1,Traits2>> operator|(const detail::iterable<Traits1>& i_lhs, const detail::iterable<Traits2>& i_rhs)
{
    return detail::iterable<detail::union_iterable_traits<Traits1,Traits2>>(detail::make_iterable_impl<detail::union_iterable_impl<detail::iterable<Traits1>,detail::iterable<Traits2>>>(i_lhs,i_rhs));
}

}
