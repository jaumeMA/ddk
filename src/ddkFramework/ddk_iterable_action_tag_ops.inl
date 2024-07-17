
namespace ddk
{

template<typename Iterable, typename ActionTag>
iterable_action_tag_result<Iterable,ActionTag> operator&&(const iterable_action_tag_result<Iterable,ActionTag>& i_lhs,const iterable_action_tag_result<Iterable,ActionTag>& i_rhs)
{
	if (static_cast<bool>(i_lhs) && static_cast<bool>(i_rhs))
	{
		return success;
	}
	else if (static_cast<bool>(i_lhs))
	{
		return make_error<iterable_action_tag_result<Iterable,ActionTag>>(i_rhs.error());
	}
	else
	{
		//we take left to right precedence
		return make_error<iterable_action_tag_result<Iterable,ActionTag>>(i_lhs.error());
	}
}

template<typename Result,typename Traits,typename ActionTag>
auto forward_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error)
{
	return i_error.template forward<Result>();
}

}