
namespace ddk
{

template<typename ActionTag, typename ... Args>
constexpr ActionTag filtered_iterable_action_cs(Args&& ... i_args)
{
    return { std::forward<Args>(i_args) ... };
}
template<>
constexpr begin_action_tag filtered_iterable_action_cs(const forward_action_tag&)
{
	return {};
}
template<>
constexpr begin_action_tag filtered_iterable_action_cs(forward_action_tag&&)
{
	return {};
}
template<>
constexpr forward_action_tag filtered_iterable_action_cs(const begin_action_tag&)
{
	return {};
}
template<>
constexpr forward_action_tag filtered_iterable_action_cs(begin_action_tag&&)
{
	return {};
}
template<>
constexpr end_action_tag filtered_iterable_action_cs(const backward_action_tag&)
{
	return {};
}
template<>
constexpr end_action_tag filtered_iterable_action_cs(backward_action_tag&&)
{
	return {};
}

}