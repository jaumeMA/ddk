#include "ddk_iterable_action_defs.h"

namespace ddk
{

action_base::action_base(bool i_valid)
: m_valid(i_valid)
{
}
action_base::operator bool() const
{
	return m_valid;
}

no_action::no_action(bool i_valid)
: action_base(i_valid)
{
}

remove_action::remove_action(bool i_valid)
: action_base(i_valid)
{
}

forward_action::forward_action(bool i_valid)
: action_base(i_valid)
{
}

backward_action::backward_action(bool i_valid)
: action_base(i_valid)
{
}

go_to_begin_action::go_to_begin_action(bool i_valid)
: forward_action(i_valid)
{
}
go_to_begin_action::go_to_begin_action(const forward_action& i_action)
: forward_action(i_action)
{
}

go_to_end_action::go_to_end_action(bool i_valid)
: backward_action(i_valid)
{
}
go_to_end_action::go_to_end_action(const backward_action& i_action)
: backward_action(i_action)
{
}

bidirectional_action::bidirectional_action(bool i_forward)
: m_forward(i_forward)
{
}
bidirectional_action::bidirectional_action(bool i_forward, bool i_valid)
: action_base(i_valid)
, m_forward(i_forward)
{
}
bidirectional_action::bidirectional_action(const displacement_action& i_action, bool i_valid)
: action_base(i_valid)
, m_forward((i_action.shift() > 0))
{
}

displacement_action::displacement_action(difference_type i_targetShift)
: m_shift(i_targetShift)
{
}
displacement_action::displacement_action(difference_type i_targetShift, bool i_valid)
: action_base(i_valid)
, m_shift(i_targetShift)
{
}
displacement_action::difference_type displacement_action::shift() const
{
	return m_shift;
}
void displacement_action::set_shift(difference_type i_shift)
{
	m_shift = i_shift;
}

}