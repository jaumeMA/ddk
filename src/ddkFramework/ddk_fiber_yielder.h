#pragma once

#include "ewas_fiber_scheduler_interface.h"
#include "ewas_thread_yielder.h"

namespace ewas
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