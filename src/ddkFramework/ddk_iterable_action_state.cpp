#include "ddk_iterable_action_state.h"

namespace ddk
{
namespace iter
{

const action_result::nested_result& action_result::get() const
{
	return m_result;
}
const action_error& action_result::error() const
{
	return m_result.get<0>();
}
bool action_result::operator==(const result_success_t&) const
{
	return m_result.is<action_error>() == false;
}
bool action_result::operator!=(const result_success_t&) const
{
	return m_result.is<action_error>();
}
bool action_result::operator==(const action_error& i_error) const
{
	return m_result.is<action_error>();
}
bool action_result::operator!=(const action_error& i_error) const
{
	return m_result.is<action_error>() == false;
}
action_result::operator bool() const
{
	return m_result.is<action_error>() == false;
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
bool action_state::operator==(const action_error& i_error) const
{
	return m_actionResult == i_error;
}
bool action_state::operator!=(const action_error& i_error) const
{
	return m_actionResult != i_error;
}

}
}