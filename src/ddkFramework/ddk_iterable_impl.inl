
namespace ddk
{
namespace detail
{

template<typename Traits, typename Iterable>
iterable_impl<Traits,Iterable>::iterable_impl(Iterable& i_iterable)
: iterable_visitor<Iterable>(i_iterable)
{
}
template<typename Traits,typename Iterable>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
iterable_result iterable_impl<Traits,Iterable>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
	loop(std::forward<Function>(i_try),i_initialAction);

	return success;
}
template<typename Traits,typename Iterable>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
iterable_result iterable_impl<Traits,Iterable>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
	loop(std::forward<Function>(i_try),i_initialAction);

	return success;
}

}
}
