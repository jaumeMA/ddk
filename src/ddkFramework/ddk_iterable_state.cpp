#include "ddk_iterable_state.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace iter
{
namespace detail
{

void action_state::set_error(action_error i_error)
{
    m_actionResult = make_error<action_result>(i_error);
}
action_result action_state::get_result() const
{
    return m_actionResult;
}
action_result action_state::extract_result()
{
    action_result res = m_actionResult;

    m_actionResult = success;

    return res;
}
void action_state::reset()
{
    m_actionResult = success;
}

}

const size_t iterable_state::npos = -1;

iterable_state::iterable_state(size_t i_initPos)
: m_currPos(i_initPos)
, m_actionState(make_shared_reference<detail::action_state>())
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
void iterable_state::forward_result(action_result i_result) const
{
    if(i_result)
    {
        m_actionState->set_error(i_result.error());
    }
}
action_result iterable_state::forward_result() const
{
    return m_actionState->extract_result();
}
bool iterable_state::operator==(const iterable_state& other) const
{
    return m_currPos == other.m_currPos;
}
bool iterable_state::operator!=(const iterable_state& other) const
{
    return m_currPos != other.m_currPos;
}

}
}
