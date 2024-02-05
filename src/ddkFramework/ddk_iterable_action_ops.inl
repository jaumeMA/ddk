
namespace ddk
{

template<typename Action,typename AAction>
and_action<Action,AAction>::and_action(const Action& i_lhs,const AAction& i_rhs, bool i_valid)
: action_base(i_valid)
, m_currentAction(i_lhs)
, m_pendingAction(i_rhs)
{
}
template<typename Action,typename AAction>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto and_action<Action,AAction>::apply(Adaptor&& i_adaptor) const
{
	typedef decltype(std::declval<Action>().apply(std::declval<Adaptor>())) return_action;
	typedef and_action<AAction,return_action> result_action;

	if (auto newAction = m_currentAction.apply(std::forward<Adaptor>(i_adaptor)))
	{
		return result_action(m_pendingAction,std::move(newAction));
	}
	else
	{
		return result_action(m_pendingAction,std::move(newAction),false);
	}
}

template<typename Action>
and_action<Action,no_action>::and_action(const Action& i_lhs, const no_action&, bool i_valid)
: action_base(i_valid)
, m_currentAction(i_lhs)
{
}
template<typename Action>
and_action<Action,no_action>::and_action(const Action& i_lhs, bool i_valid)
: action_base(i_valid)
, m_currentAction(i_lhs)
{
}
template<typename Action>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto and_action<Action,no_action>::apply(Adaptor&& i_adaptor) const
{
	return m_currentAction.apply(std::forward<Adaptor>(i_adaptor));
}

template<typename Action>
and_action<no_action,Action>::and_action(const no_action&, const Action& i_rhs, bool i_valid)
: action_base(i_valid)
, m_currentAction(i_rhs)
{
}
template<typename Action>
and_action<no_action,Action>::and_action(const Action& i_rhs, bool i_valid)
: action_base(i_valid)
, m_currentAction(i_rhs)
{
}
template<typename Action>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto and_action<no_action,Action>::apply(Adaptor&& i_adaptor) const
{
	return m_currentAction.apply(std::forward<Adaptor>(i_adaptor));
}

TEMPLATE(typename Action,typename AAction)
REQUIRED(IS_ITERABLE_ACTION(Action),IS_ITERABLE_ACTION(AAction))
auto operator&&(Action&& i_lhs,AAction&& i_rhs)
{
	return and_action{i_lhs,i_rhs};
}
TEMPLATE(typename Action)
REQUIRED(IS_ITERABLE_ACTION(Action))
auto operator&&(Action&& i_lhs,const no_action&)
{
	return i_lhs;
}
TEMPLATE(typename Action)
REQUIRED(IS_ITERABLE_ACTION(Action))
auto operator&&(const no_action&,Action&& i_rhs)
{
	return i_rhs;
}

}