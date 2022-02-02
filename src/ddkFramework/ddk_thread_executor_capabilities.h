#pragma once

#include "ddk_executor_capabilities.h"
#include "ddk_thread_executor_interface.h"
#include "ddk_thread.h"
#include "ddk_thread_sheaf.h"

namespace ddk
{
namespace detail
{

template<>
struct executor_capabilities<ddk::thread> : public thread_executor_interface
{
public:
	executor_capabilities() = default;
	executor_capabilities(ddk::thread i_context);
	executor_capabilities(const executor_capabilities&) = default;
	executor_capabilities(executor_capabilities&&) = default;

	bool set_affinity(const cpu_set_t& i_set) override;

protected:
	ddk::thread m_context;
};

template<>
struct executor_capabilities<ddk::thread_sheaf> : public thread_executor_interface
{
public:
	executor_capabilities() = default;
	executor_capabilities(ddk::thread_sheaf i_context);
	executor_capabilities(const executor_capabilities&) = default;
	executor_capabilities(executor_capabilities&&) = default;

	bool set_affinity(const cpu_set_t& i_set) override;

protected:
	ddk::thread_sheaf m_context;
};

}
}

#include "ddk_thread_executor_capabilities.inl"