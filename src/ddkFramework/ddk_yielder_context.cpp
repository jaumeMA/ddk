#include "ddk_yielder_context.h"


namespace ddk
{
namespace detail{

yielder_context::yielder_context(fiber_id i_id, State i_state)
: m_state(std::make_pair(i_id,i_state))
{
}
void yielder_context::stop(fiber_id i_id)
{
    m_state = std::make_pair(i_id,Stopped);
}
bool yielder_context::is_stopped(fiber_id i_id) const
{
    return m_state.first == i_id && m_state.second == Stopped;
}
void yielder_context::pause(fiber_id i_id)
{
    m_state = std::make_pair(i_id,Paused);
}
bool yielder_context::is_paused(fiber_id i_id) const
{
    return m_state.first == i_id && m_state.second == Paused;
}
bool yielder_context::is_running(fiber_id i_id) const
{
    return m_state.first == i_id && m_state.second == Running;
}

}
}
