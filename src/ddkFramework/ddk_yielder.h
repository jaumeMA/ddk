#pragma once

#include "ddk_yielder_interface.h"
#include "ddk_yielder_context.h"

namespace ddk
{
namespace detail
{

struct yielder
{
public:
	explicit yielder(yielder_interface& i_executor);
	void yield(yielder_context* i_context);
	void suspend();

private:
	yielder_interface& m_executor;
};

}
}
