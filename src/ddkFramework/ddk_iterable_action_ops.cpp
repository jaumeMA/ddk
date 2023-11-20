#include "ddk_iterable_action_defs.h"

namespace ddk
{

backward_action inverse(const forward_action&)
{
	return {};
}
forward_action inverse(const backward_action&)
{
	return {};
}
go_to_begin_action inverse(const go_to_end_action&)
{
	return {};
}
go_to_end_action inverse(const go_to_begin_action&)
{
	return {};
}
displacement_action inverse(const displacement_action& i_action)
{
	return i_action.shift();
}

}