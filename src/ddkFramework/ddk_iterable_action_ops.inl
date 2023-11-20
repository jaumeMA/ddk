
namespace ddk
{

TEMPLATE(typename Action,typename AAction)
REQUIRED(IS_ITERABLE_ACTION(Action),IS_ITERABLE_ACTION(AAction))
auto operator&&(const Action& i_lhs,const AAction& i_rhs)
{
	return action_list{i_lhs,i_rhs};
}

}