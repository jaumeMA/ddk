#include "ddk_iterable_state.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace iter
{
namespace detail
{

void action_state::set(action_result i_result)
{
    m_actionResult = i_result;
}
action_result action_state::get() const
{
    return m_actionResult;
}
action_result action_state::extract()
{
    action_result res = m_actionResult;

    m_actionResult = success;

    return res;
}
void action_state::reset()
{
    m_actionResult = success;
}

defaul_iterable_pointer::defaul_iterable_pointer(bool i_forward)
: m_forward(i_forward)
{
}
defaul_iterable_pointer::distance defaul_iterable_pointer::distance_to_next(int i_next) const
{
    if(m_forward)
    {
        return i_next;
    }
    else
    {
        return -i_next;
    }
}
defaul_iterable_pointer::distance defaul_iterable_pointer::distance_to_prev(int i_next) const
{
    if(m_forward)
    {
        return -i_next;
    }
    else
    {
        return i_next;
    }
}

}

const size_t iterable_state::npos = -1;

iterable_state::iterable_state(size_t i_initPos, bool i_forward)
: m_currPos(i_initPos)
, m_actionState(make_shared_reference<detail::action_state>())
, m_iterablePointer(make_shared_reference<detail::defaul_iterable_pointer>(i_forward))
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
void iterable_state::forward_result(action_result i_result)
{
    m_actionState->set(i_result);
}
action_result iterable_state::forward_result()
{
    return m_actionState->extract();
}
bool iterable_state::operator==(const iterable_state& other) const
{
    return m_currPos == other.m_currPos;
}
bool iterable_state::operator!=(const iterable_state& other) const
{
    return m_currPos != other.m_currPos;
}
int iterable_state::distance_to_next(int i_next) const
{
    return m_iterablePointer->distance_to_next(i_next);
}
int iterable_state::distance_to_prev(int i_next) const
{
    return m_iterablePointer->distance_to_prev(i_next);
}

}
}
