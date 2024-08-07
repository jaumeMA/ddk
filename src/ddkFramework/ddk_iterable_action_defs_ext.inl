
namespace ddk
{

template<typename ... Actions>
TEMPLATE(typename AAction)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(AAction,Actions...))
constexpr any_action<Actions...>::any_action(AAction&& i_action, bool i_valid)
: action_base(i_valid)
, m_actions(std::forward<AAction>(i_action))
{
}
template<typename ... Actions>
TEMPLATE(typename Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
constexpr auto any_action<Actions...>::apply(Adaptor&& i_adaptor) const
{
	return visit([&](auto&& i_action)
	{
		return i_action.apply(std::forward<Adaptor>(i_adaptor));
	},m_actions);
}

constexpr swap_action::swap_action(bool i_valid)
: action_base(i_valid)
{
}

}