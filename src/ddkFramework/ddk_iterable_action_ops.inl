
namespace ddk
{

template<typename Action,typename AAction>
and_action<Action,AAction>::and_action(const Action& i_lhs,const AAction& i_rhs)
: Action(i_lhs)
, m_pendingAction(i_rhs)
{
}
template<typename Action,typename AAction>
and_action<Action,AAction>::and_action(const stop_action& i_action)
: Action(i_action)
, m_pendingAction(i_action)
{
}
template<typename Action,typename AAction>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto and_action<Action,AAction>::apply(Adaptor&& i_adaptor) const
{
	if (const auto actionRes = Action::apply(std::forward<Adaptor>(i_adaptor)))
	{
		typedef decltype(m_pendingAction&& actionRes.get()) result_action;

		return make_result<iterable_action_result<result_action>>(m_pendingAction && actionRes.get());
	}
	else
	{
		return make_error<iterable_action_result<AAction>>(IterableError::InternalError,"Could not pair action");
	}
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

TEMPLATE(typename Action)
REQUIRED(IS_ITERABLE_ACTION(Action))
auto operator&&(Action&& i_lhs,const stop_action&)
{
	return no_action{};
}

TEMPLATE(typename Action)
REQUIRED(IS_ITERABLE_ACTION(Action))
auto operator&&(const stop_action&,Action&& i_rhs)
{
	return no_action{};
}

}