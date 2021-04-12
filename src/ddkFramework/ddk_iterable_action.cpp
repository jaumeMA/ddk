#include "ddk_iterable_action.h"

namespace ddk
{

bool operator==(const base_action& i_lhs, const base_action& i_rhs)
{
	return true;
}
bool operator!=(const base_action& i_lhs, const base_action& i_rhs)
{
    return false;
}
bool operator==(const shift_action& i_lhs,const shift_action& i_rhs)
{
	return i_lhs.shifting() == i_rhs.shifting();
}
bool operator!=(const shift_action& i_lhs,const shift_action& i_rhs)
{
	return i_lhs.shifting() != i_rhs.shifting();
}

const stop_action stop_iteration = stop_action();
const erase_action erase_value = erase_action();
const add_action add_value = add_action();
const go_forward_action go_next_place = go_forward_action();
const go_backward_action go_prev_place = go_backward_action();
const shift_action go_to_place = shift_action(0);
const shift_action go_no_place = shift_action(0);

ShiftActionError::ShiftActionError(difference_type i_pendingShift)
: m_pendingShift(i_pendingShift)
{
}
typename ShiftActionError::difference_type ShiftActionError::get_pending_shift() const
{
	return m_pendingShift;
}

}
