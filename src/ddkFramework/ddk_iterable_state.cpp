#include "ddk_iterable_state.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace iter
{

action_state::action_state()
: m_actionResult(make_result<action_result>(go_no_place))
{
	DDK_ASSERT(m_actionResult == success,"forced check");
}
action_state::~action_state()
{
	m_actionResult.clear();
}
action_result action_state::get() const
{
    return m_actionResult;
}
action_result action_state::extract()
{
    return m_actionResult.extract();
}
void action_state::reset()
{
    m_actionResult.clear();
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

const size_t iterable_state::npos = -1;

iterable_state::iterable_state(size_t i_pos)
: m_currPos(i_pos)
{
}
size_t iterable_state::position() const
{
    return m_currPos;
}
void iterable_state::reset()
{
    m_currPos = npos;
}
bool iterable_state::operator==(const iterable_state& other) const
{
    return m_currPos == other.m_currPos;
}
bool iterable_state::operator!=(const iterable_state& other) const
{
    return m_currPos != other.m_currPos;
}
bool iterable_state::operator==(const result_success_t&) const
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	return actionResult.get() == success;
}
bool iterable_state::operator!=(const result_success_t&) const
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	return actionResult.get() != success;
}
bool iterable_state::is_success()
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	return actionResult.get() == success;
}
bool iterable_state::flush_result()
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	if(actionResult.empty() == false)
	{
		if(const action_result actionRes = actionResult.extract())
		{
			return true;
		}
	}

	return false;
}
action_result iterable_state::forward_result()
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	return actionResult.extract();
}
void iterable_state::forward_result(action_result i_result)
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	flush_result();

	actionResult.set(std::move(i_result));
}
fiberlocal<action_result,iterable_state>& iterable_state::get_action_result()
{
	static fiberlocal<action_result,iterable_state> s_actionResult;

	return s_actionResult;
}

}
}
