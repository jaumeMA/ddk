#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{

shift_action::shift_action(int i_targetShift,int i_currShift,bool i_stepByStep)
: m_targetShift(i_targetShift)
, m_currShift(i_currShift)
, m_stepByStep(i_stepByStep)
{
}
shift_action::shift_action(shift_action&& other)
: m_targetShift(other.m_targetShift)
, m_currShift(other.m_currShift)
, m_stepByStep(other.m_stepByStep)
{
	other.m_currShift = other.m_targetShift;
}
int shift_action::shifted() const
{
    return m_currShift;
}
int shift_action::shifting() const
{
	if(m_stepByStep && m_targetShift > 0)
	{
		return (m_targetShift > 0) ? 1 : -1;
	}
	else
	{
		return m_targetShift - m_currShift;
	}
}
shift_action shift_action::operator()(int i_targetShift,int i_currShift) const
{
    return shift_action(i_targetShift,i_currShift);
}
bool shift_action::apply(const shift_action& i_appliedAction)
{
	m_currShift += i_appliedAction.m_currShift;

	return m_currShift == m_targetShift;
}
void shift_action::set_step_by_step(bool i_cond)
{
	m_stepByStep = i_cond;
}
bool shift_action::step_by_step() const
{
	return m_stepByStep;
}
shift_action& shift_action::operator=(const shift_action& other)
{
	m_targetShift = other.m_targetShift;
	m_currShift = other.m_currShift;
	m_stepByStep = other.m_stepByStep;

	return *this;
}
shift_action& shift_action::operator=(shift_action&& other)
{
	m_targetShift = other.m_targetShift;
	m_currShift = other.m_currShift;
	m_stepByStep = other.m_stepByStep;

	other.m_currShift = other.m_targetShift;

	return *this;
}
shift_action::operator bool() const
{
	return m_targetShift == m_currShift;
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

const iter::stop_action stop_iteration = iter::stop_action();
const iter::erase_action erase_value = iter::erase_action();
const iter::add_action add_value = iter::add_action();
const iter::go_forward_action go_next_place = iter::go_forward_action();
const iter::go_backward_action go_prev_place = iter::go_backward_action();
const iter::shift_action go_to_place = iter::shift_action(0);
const iter::shift_action go_no_place = iter::shift_action(0);

}
}
