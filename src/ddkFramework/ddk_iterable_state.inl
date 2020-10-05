
namespace ddk
{
namespace iter
{

template<typename Action>
void iterable_state::apply(const Action& i_action)
{
    detail::state_visitor<Action> actionVisitor(m_currPos);

    i_action.visit(actionVisitor);
}

}
}
