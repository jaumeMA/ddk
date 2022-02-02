#pragma once

#include "ddk_context_executor_interface.h"

namespace ddk
{
namespace detail
{

template<typename Context>
struct executor_capabilities : public context_executor_interface
{
public:
	executor_capabilities() = default;
	executor_capabilities(Context i_context);
	executor_capabilities(const executor_capabilities&) = default;
	executor_capabilities(executor_capabilities&&) = default;

	//none

protected:
	Context m_context;
};

}
}

#include "ddk_executor_capabilities.inl"