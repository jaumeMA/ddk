
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
template<typename Action>
void iterable_impl<Traits,Iterable>::iterate_impl(const Action& i_initialAction)
{
	loop(i_initialAction);
}
template<typename Traits,typename Iterable>
template<typename Action>
void iterable_impl<Traits,Iterable>::iterate_impl(const Action& i_initialAction) const
{
	loop(i_initialAction);
}

}
}
