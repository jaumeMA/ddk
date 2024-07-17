#include "ddk_iterable_action.h"

namespace ddk
{

const go_to_begin_action go_to_begin = go_to_begin_action(true);
const go_to_end_action go_to_end = go_to_end_action(true);
const forward_action go_next_place = forward_action(true);
const backward_action go_prev_place = backward_action(true);
const remove_action remove_place = remove_action(true);

}
