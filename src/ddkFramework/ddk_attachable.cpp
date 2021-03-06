#include "ddk_attachable.h"

namespace ddk
{

attachable<void> attach(thread i_thread)
{
    return attachable<void>(make_executor<detail::thread_executor<detail::void_t>>(std::move(i_thread)));
}
attachable<void> attach(fiber i_fiber)
{
    return attachable<void>(make_executor<detail::fiber_executor<detail::void_t>>(std::move(i_fiber)));
}
attachable<void> attach(thread_sheaf i_threadSheaf)
{
    return attachable<void>(make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf)));
}
attachable<void> attach(fiber_sheaf i_fiberSheaf)
{
    return attachable<void>(make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf)));
}
attachable<void> attach(cancellable_executor_unique_ref<detail::void_t> i_attachable)
{
	return attachable<void>(std::move(i_attachable));
}

}
