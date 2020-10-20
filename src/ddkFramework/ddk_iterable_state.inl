
namespace ddk
{
namespace iter
{

template<typename Action>
void iterable_state::apply(const Action& i_action)
{
    detail::state_visitor<Action> actionVisitor(m_currPos);

    m_currPos = i_action.visit(actionVisitor);
}
template<typename Result>
Result iterable_state::forward_result_as() const
{
    if(action_result currRes = m_actionState->extract_result())
    {
        return success;
    }
    else
    {
        return make_error<Result>(currRes.error().get_nested_error<typename Result::error_t>());
    }
}

}
}
