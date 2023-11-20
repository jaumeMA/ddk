#include "ddk_iterable_action.h"

namespace ddk
{

shift_error::shift_error(const ShiftError& i_error,int i_pendingShift)
: error<ShiftError>(i_error)
, _pendingShift(i_pendingShift)
{
}
shift_error::shift_error(const ShiftError& i_error,const std::string& i_errorDesc, long long int i_pendingShift)
: error<ShiftError>(i_error,i_errorDesc)
, _pendingShift(i_pendingShift)
{
}
long long int shift_error::get_pending_shift() const
{
	return _pendingShift;
}

stop_error::stop_error(const StopError& i_error,int i_code)
: error<StopError>(i_error)
, _code(i_code)
{
}
stop_error::stop_error(const StopError& i_error,const std::string& i_errorDesc,int i_code)
: error<StopError>(i_error,i_errorDesc)
,_code(i_code)
{
}

const stop_action stop_iteration = stop_action();
const go_to_begin_action go_to_begin = go_to_begin_action();
const go_to_end_action go_to_end = go_to_end_action();
const forward_action go_next_place = forward_action();
const backward_action go_prev_place = backward_action();

}
