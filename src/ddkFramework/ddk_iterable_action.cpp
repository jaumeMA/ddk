#include "ddk_iterable_action.h"

namespace ddk
{
namespace iter
{

int shift_action::shifted() const
{
    return m_shift;
}
shift_action shift_action::operator()(int i_shift) const
{
    shift_action res;

    res.m_shift = i_shift;

    return res;
}

const iter::stop_action stop_iteration = iter::stop_action();
const iter::erase_action erase_value = iter::erase_action();
const iter::add_action add_value = iter::add_action();
const iter::go_forward_action go_next_place = iter::go_forward_action();
const iter::go_backward_action go_prev_place = iter::go_backward_action();
const iter::shift_action go_shift_place = iter::shift_action();

}
}