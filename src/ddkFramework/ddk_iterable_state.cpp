#include "ddk_iterable_state.h"

namespace ddk
{
namespace iter
{

size_t iterable_state::position() const
{
    return m_currPos;
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
