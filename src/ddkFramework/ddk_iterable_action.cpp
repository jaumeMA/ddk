#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{

shift_action::shift_action(int i_shift)
: m_targetShift(i_shift)
, m_currShift(0)
{
}
int shift_action::shifted() const
{
    return m_currShift;
}
int shift_action::shifting() const
{
	return m_targetShift;
}
int shift_action::incremental_shift() const
{
	return (m_targetShift > 0) ? 1 : -1;
}
shift_action shift_action::operator()(int i_shift) const
{
    return shift_action(i_shift);
}
bool shift_action::apply(const shift_action& i_appliedAction)
{
	m_currShift += i_appliedAction.m_targetShift;

	return m_currShift == m_targetShift;
}

go_forward_action::go_forward_action()
: shift_action(1)
{
}
go_forward_action::go_forward_action(const shift_action& other)
: shift_action(other)
{
}

go_backward_action::go_backward_action()
: shift_action(-1)
{
}
go_backward_action::go_backward_action(const shift_action& other)
: shift_action(other)
{
}

bool operator==(const forward_action& i_lhs, const forward_action& i_rhs)
{
    return i_lhs.which() == i_rhs.which();
}
bool operator==(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs)
{
    return i_lhs.which() == i_rhs.which();
}
bool operator==(const random_access_action& i_lhs, const random_access_action& i_rhs)
{
    return i_lhs.which() == i_rhs.which();
}
bool operator!=(const forward_action& i_lhs, const forward_action& i_rhs)
{
    return i_lhs.which() != i_rhs.which();
}
bool operator!=(const bidirectional_action& i_lhs, const bidirectional_action& i_rhs)
{
    return i_lhs.which() != i_rhs.which();
}
bool operator!=(const random_access_action& i_lhs, const random_access_action& i_rhs)
{
    return i_lhs.which() != i_rhs.which();
}

const iter::no_op_action no_op = iter::no_op_action();
const iter::stop_action stop_iteration = iter::stop_action();
const iter::erase_action erase_value = iter::erase_action();
const iter::add_action add_value = iter::add_action();
const iter::go_forward_action go_next_place = iter::go_forward_action();
const iter::go_backward_action go_prev_place = iter::go_backward_action();
const iter::shift_action go_to_place = iter::shift_action(0);
const iter::shift_action go_no_place = iter::shift_action(0);

}
}
