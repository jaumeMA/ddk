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
			return ddk::make_error<iteration_result>(actionError.template get_nested_error<stop_error>());
		}
		else
		{
			//any other error is considered to be not of client bussiness
			return success;
		}
	}
}

}

iteration_result::iteration_result(const action_result& i_result)
: result<void,stop_error>(resolve_result(i_result))
{
}
iteration_result::~iteration_result()
{
	//no mandatory to check this result
	dismiss();
}

}