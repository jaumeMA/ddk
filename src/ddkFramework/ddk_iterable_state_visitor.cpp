#include "ddk_iterable_state_visitor.h"

namespace ddk
{

size_t action_shift_visitor::operator()(const shift_action& i_action) const
{
	return m_currPos + i_action.shifting();
}

bool consolidate_visitor::operator()(shift_action& i_action) const
{
	return i_action.apply(m_appliedAction);
}

}
