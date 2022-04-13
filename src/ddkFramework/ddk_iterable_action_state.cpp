#include "ddk_iterable_action_state.h"

namespace ddk
{

action_result::~action_result()
{
	//in this case its not mandatory to check result
	dismiss();
}
bool action_result::operator==(const result_success_t&) const
{
	return operator bool();
}
bool action_result::operator!=(const result_success_t&) const
{
	return operator bool() == false;
}

action_state::action_state()
: m_actionResult(go_no_place)
{
}
action_result action_state::get() const
{
	return m_actionResult;
}
bool action_state::operator==(result_success_t) const
{
	return m_actionResult == success;
}
bool action_state::operator!=(result_success_t) const
{
	return m_actionResult != success;
}

}
