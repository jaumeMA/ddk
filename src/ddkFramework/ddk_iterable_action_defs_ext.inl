
namespace ddk
{

template<typename ... Actions>
TEMPLATE(typename AAction)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(AAction,Actions...))
any_action<Actions...>::any_action(AAction&& i_action)
: Actions(stop_iteration) ...
, m_actions(std::forward<AAction>(i_action))
{
}
template<typename ... Actions>
TEMPLATE(typename Adaptor,typename Sink)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
auto any_action<Actions...>::apply(Adaptor&& i_adaptor,Sink&& i_sink) const
{
	return visit([&](auto&& i_action)
	{
		return i_action.apply(std::forward<Adaptor>(i_adaptor),std::forward<Sink>(i_sink));
	},m_actions);
}

}