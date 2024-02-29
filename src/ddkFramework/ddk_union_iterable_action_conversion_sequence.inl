
namespace ddk
{

template<typename ActionTag>
constexpr inline ActionTag&& union_iterable_action_cs(ActionTag&& i_action)
{
	return std::forward<ActionTag>(i_action);
}
template<>
constexpr begin_action_tag union_iterable_action_cs(const begin_next_iterable&)
{
	return {};
}
template<>
constexpr begin_action_tag union_iterable_action_cs(begin_next_iterable&&)
{
	return {};
}
template<>
constexpr forward_action_tag union_iterable_action_cs(const begin_next_iterable&)
{
	return {};
}
template<>
constexpr forward_action_tag union_iterable_action_cs(begin_next_iterable&&)
{
	return {};
}
template<>
constexpr begin_next_iterable union_iterable_action_cs(const begin_action_tag&)
{
	return {};
}
template<>
constexpr begin_next_iterable union_iterable_action_cs(begin_action_tag&&)
{
	return {};
}
template<>
constexpr begin_next_iterable union_iterable_action_cs(const forward_action_tag&)
{
	return {};
}
template<>
constexpr begin_next_iterable union_iterable_action_cs(forward_action_tag&&)
{
	return {};
}
template<>
constexpr end_prev_iterable union_iterable_action_cs(const backward_action_tag&)
{
	return {};
}
template<>
constexpr end_prev_iterable union_iterable_action_cs(backward_action_tag&&)
{
	return {};
}
template<>
constexpr end_prev_iterable union_iterable_action_cs(const end_action_tag&)
{
	return {};
}
template<>
constexpr end_prev_iterable union_iterable_action_cs(end_action_tag&&)
{
	return {};
}
template<>
constexpr backward_action_tag union_iterable_action_cs(const end_prev_iterable&)
{
	return {};
}
template<>
constexpr backward_action_tag union_iterable_action_cs(end_prev_iterable&&)
{
	return {};
}
template<typename ActionTag,typename ... Args>
constexpr ActionTag union_iterable_action_cs(Args&& ... i_args)
{
	return { std::forward<Args>(i_args) ... };
}

}