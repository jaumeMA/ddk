
namespace ddk
{

TEMPLATE(typename Action,typename Iterable)
REQUIRED(IS_ITERABLE_ACTION(Action),IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
void operator>>=(Action&& i_action,Iterable&& i_iterable)
{
	ddk::deduce_iterable(std::forward<Iterable>(i_iterable)).iterate_impl(std::forward<Action>(i_action));
}

}