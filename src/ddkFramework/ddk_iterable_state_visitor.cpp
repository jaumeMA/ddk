#include "ddk_iterable_state_visitor.h"

namespace ddk
{
namespace iter
{

size_t action_shift_visitor::visit(const shift_action& i_action) const
{
	return m_currPos + i_action.shifting();
}

bool consolidate_visitor::visit(shift_action& i_action) const
{
	return i_action.apply(m_appliedAction);
}

}
}