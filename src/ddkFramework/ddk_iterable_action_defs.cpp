#include "ddk_iterable_action_defs.h"
#include "ddk_fiber_utils.h"

namespace ddk
{

void stop_action::operator()() const
{
	suspend();
}

shift_action::shift_action(difference_type i_targetShift,difference_type i_currShift,bool i_stepByStep)
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
typename shift_action::difference_type shift_action::shifted() const
{
	return m_currShift;
}
typename shift_action::difference_type shift_action::target_shift() const
{
	return m_targetShift;
}
typename shift_action::difference_type shift_action::shifting() const
{
	if(m_stepByStep && m_targetShift != m_currShift)
	{
		return (m_targetShift > 0) ? 1 : -1;
	}
	else
	{
		return m_targetShift - m_currShift;
	}
}
shift_action shift_action::operator()(difference_type i_targetShift,difference_type i_currShift) const
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
	if(m_stepByStep != i_cond)
	{
		m_stepByStep = i_cond;

		if(i_cond && m_targetShift == 0)
		{
			m_targetShift = 1;
		}
	}
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

}