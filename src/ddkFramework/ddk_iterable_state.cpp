#include "ddk_iterable_state.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace iter
{

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

}
}
