
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename IterableImpl, typename ... Args>
iterable_impl_shared_ref<typename IterableImpl::traits> make_iterable_impl(Args&& ... i_args)
{
    return make_shared_reference<IterableImpl>(std::forward<Args>(i_args) ...);
}

}
}
