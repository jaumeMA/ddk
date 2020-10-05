
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_async.h"

namespace ddk
{

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

}
