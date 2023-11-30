#include "ddk_iterable_action.h"

namespace ddk
{

const stop_action stop_iteration = stop_action();
const go_to_begin_action go_to_begin = go_to_begin_action();
const go_to_end_action go_to_end = go_to_end_action();
const forward_action go_next_place = forward_action();
const backward_action go_prev_place = backward_action();
const remove_action remove_place = remove_action();

}
