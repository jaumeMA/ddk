#pragma once

#include "ddk_yielder_context.h"

namespace ddk
{
namespace detail
{

class yielder_interface
{
protected:
	virtual ~yielder_interface() = default;

public:
	virtual void yield(yielder_context*) = 0;
	virtual bool suspend() = 0;
};

}
}
