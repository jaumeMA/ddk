#pragma once

#include "ddk_iterable_action_state.h"
#include "ddk_scoped_enum.h"

namespace ddk
{

struct iteration_result : public result<void,stop_error>
{
public:
	using result<void,stop_error>::result;
	iteration_result(const action_result& i_result);
	~iteration_result();
};

}