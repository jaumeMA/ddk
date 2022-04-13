#include "ddk_iteration_result.h"

namespace ddk
{
namespace
{

iteration_result resolve_result(const action_result& i_result)
{
	if(i_result)
	{
		return success;
	}
	else
	{
		const action_error& actionError = i_result.error();

		if(actionError.get_error() == ActionError::StopError)
		{
			return ddk::make_error<iteration_result>(IterationError::Stop,actionError.template get_nested_error<stop_error>());
		}
		else
		{
			return ddk::make_error<iteration_result>(IterationError::Internal,IterationInternalError::Error);
		}
	}
}

}

iteration_result::iteration_result(const action_result& i_result)
: result<void,iteration_error>(resolve_result(i_result))
{
}
iteration_result::~iteration_result()
{
	//no mandatory to check this result
	dismiss();
}

}