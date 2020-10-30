
#include "ddk_iterable_state_visitor.h"

namespace ddk
{
namespace iter
{

template<typename Action>
void iterable_state::apply(const Action& i_action)
{
	action_shift_visitor actionVisitor(m_currPos);

    m_currPos = i_action.visit(actionVisitor);
}
template<typename Result>
Result iterable_state::forward_result_as()
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	const action_result currRes = actionResult.extract();
	if(currRes != success)
    {
        return success;
    }
    else
    {
        return make_error<Result>(currRes.error().get_nested_error<typename Result::error_t>());
    }
}
template<typename ErrorCode>
bool iterable_state::in_error(ErrorCode&& i_error)
{
	fiberlocal<action_result,iterable_state>& actionResult = get_action_result();

	return actionResult.get() == action_error(i_error);
}

}
}
