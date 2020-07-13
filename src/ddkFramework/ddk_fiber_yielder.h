#pragma once

#include "ddk_fiber_scheduler_interface.h"
#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

struct fiber_yielder : yielder
{
public:
	explicit fiber_yielder(fiber_yielder_interface& i_executor);
	void yield(yielder_context*) override;
	void suspend(yielder_context* = nullptr) override;

private:
	fiber_yielder_interface& m_executor;
};

}
}