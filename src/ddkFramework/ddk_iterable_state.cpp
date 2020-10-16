#include "ddk_iterable_state.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace iter
{
namespace detail
{

void action_state::set_error(IterableStateError i_error) const
{
    m_error = i_error;
}
IterableStateError action_state::get_error() const
{
    return m_error;
}
void action_state::reset() const
{
    m_error = IterableStateError::None;
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
void iterable_state::produce_error(IterableStateError i_error) const
{
    m_actionState->set_error(i_error);
}
IterableStateError iterable_state::consume_error() const
{
    IterableStateError res = m_actionState->get_error();

    m_actionState->reset();

    return res;
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
