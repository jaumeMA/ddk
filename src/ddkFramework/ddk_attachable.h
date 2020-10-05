#pragma once

#include "ddk_sync_executor_impl.h"

namespace ddk
{

template<typename>
class attachable;
template<typename RReturn>
class async_executor;

template<typename Return>
class attachable
{
    friend class async_executor<Return>;

public:
    attachable(cancellable_executor_unique_ref<Return> i_executorImpl);
    attachable(const attachable&) = delete;
    attachable(attachable&&) = default;
	~attachable() = default;

private:
    cancellable_executor_unique_ref<Return> m_executorImpl;
};

template<>
class attachable<void> : public attachable<detail::void_t>
{
public:
    using attachable<detail::void_t>::attachable;

    attachable(const attachable&) = delete;
    attachable(attachable&&) = default;
	~attachable() = default;
};

attachable<void> attach(thread i_thread);
attachable<void> attach(const detail::this_thread_t&);
attachable<void> attach(fiber i_fiber);
attachable<void> attach(const detail::this_fiber_t&);
attachable<void> attach(thread_sheaf i_threadSheaf);
attachable<void> attach(fiber_sheaf i_fiberSheaf);

}

#include "ddk_attachable.inl"
