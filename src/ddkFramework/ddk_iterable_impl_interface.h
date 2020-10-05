#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_impl_interface
{
public:
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;
    typedef Traits traits;

    virtual ~iterable_impl_interface() = default;

    virtual void iterate_impl(const function<action(reference)>& i_try) = 0;
    virtual void iterate_impl(const function<action(const_reference)>& i_try) const = 0;
};

template<typename Traits>
using iterable_impl_shared_ref = shared_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_shared_ref = shared_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_shared_ptr = shared_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_shared_ptr = shared_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename Traits>
using iterable_impl_lent_ref = lent_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ref = lent_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_lent_ptr = lent_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ptr = lent_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename T>
using forward_iterable_impl_interface = iterable_impl_interface<detail::forward_iterable_traits<T>>;

template<typename T>
using forward_iterable_impl_shared_ref = shared_reference_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_shared_ref = shared_reference_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_shared_ptr = shared_pointer_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_shared_ptr = shared_pointer_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;

template<typename T>
using forward_iterable_impl_lent_ref = lent_reference_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_lent_ref = lent_reference_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_lent_ptr = lent_pointer_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_lent_ptr = lent_pointer_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;

template<typename IterableImpl, typename ... Args>
inline iterable_impl_shared_ref<typename IterableImpl::traits> make_iterable_impl(Args&& ... i_args);

}
}

#include "ddk_iterable_impl_interface.inl"