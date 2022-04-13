#pragma once

#include "ddk_iterable_action_state.h"
#include "ddk_scoped_enum.h"

namespace ddk
{

SCOPED_ENUM_DECL(IterationInternalError,
				Error);
SCOPED_ENUM_DECL(IterationError,
				Internal,
				Stop);

typedef error<IterationError,IterationInternalError,stop_error> iteration_error;

struct iteration_result : public result<void,iteration_error>
{
public:
	using result<void,iteration_error>::result;
	iteration_result(const action_result& i_result);
	~iteration_result();
};

}