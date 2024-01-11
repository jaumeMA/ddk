
namespace ddk
{

template<typename ActionTag,typename ... Args>
ActionTag union_iterable_action_cs(Args&& ... i_args)
{
	return { std::forward<Args>(i_args) ... };
}

}