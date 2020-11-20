
namespace ddk
{
namespace detail
{

template<size_t Index, typename ... Iterables>
typename mpl::nth_type_of<Index, Iterables...>::type::reference intersection_navigate(intersection_iterable_impl<Iterables...>* i_iterable, const iter::shift_action& i_initialAction)
{
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::reference curr_reference;
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::iterable_value curr_iterable_value;

	i_iterable->m_iterables.template get<Index>().iterate(make_function([](curr_iterable_value i_value){ yield<curr_reference>(*i_value); }),nullptr,i_initialAction);

	suspend();

	return crash_on_return<curr_reference>::value();
}
template<size_t Index, typename ... Iterables>
typename mpl::nth_type_of<Index, Iterables...>::type::const_reference const_intersection_navigate(const intersection_iterable_impl<Iterables...>* i_iterable, const iter::shift_action& i_initialAction)
{
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::const_reference curr_const_reference;
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::iterable_const_value curr_iterable_const_value;

	i_iterable->m_iterables.template get<Index>().iterate(make_function([](curr_iterable_const_value i_value) { yield<curr_const_reference>(*i_value); }),nullptr,i_initialAction);

	suspend();

	return crash_on_return<curr_const_reference>::value();
}

template<typename ... Iterables>
intersection_iterable_impl<Iterables...>::intersection_iterable_impl(const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
{
}
template<typename ... Iterables>
intersection_iterable_impl<Iterables...>::intersection_iterable_impl(const tuple<Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
{
}
template<typename ... Iterables>
const tuple<Iterables...>& intersection_iterable_impl<Iterables...>::get_iterables() const
{
    return m_iterables;
}
template<typename ... Iterables>
tuple<Iterables...>& intersection_iterable_impl<Iterables...>::get_iterables()
{
    return m_iterables;
}
template<typename ... Iterables>
void intersection_iterable_impl<Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
void intersection_iterable_impl<Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
size_t intersection_iterable_impl<Iterables...>::size() const
{
	TODO("Pending");
	
	return 0;
}
template<typename ... Iterables>
bool intersection_iterable_impl<Iterables...>::empty() const
{
	TODO("Pending");
	
	return true;
}
template<typename ... Iterables>
template<size_t ... Indexs>
void intersection_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr)
{
    tuple<awaitable<typename Iterables::reference>...> awaitableTuple(await(make_function(intersection_navigate<Indexs,Iterables...>,this,i_initialAction))...);
    tuple<awaited_result<typename Iterables::reference>...> awaitableResultTuple;
    action currAction = traits::default_action();

    do
    {
        if((awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...))
        {
            currAction = eval(i_try,make_tuple(awaitableResultTuple.template get<Indexs>().get() ...));

			//update current action
			(m_iterables.template get<Indexs>().forward_action(currAction) && ...);
        }
        else
        {
            break;
        }
    }
    while(true);
}
template<typename ... Iterables>
template<size_t ... Indexs>
void intersection_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr) const
{
    tuple<awaitable<typename Iterables::const_reference>...> awaitableTuple(await(make_function(const_intersection_navigate<Indexs,Iterables...>,this,i_initialAction))...);
    tuple<awaited_result<typename Iterables::const_reference>...> awaitableResultTuple;
    action currAction = traits::default_action();

    do
    {
        (m_iterables.template get<Indexs>().forward_action(currAction) && ...);

        if((awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...))
        {
            currAction = eval(i_try,make_tuple(awaitableResultTuple.template get<Indexs>().get() ...));
        }
        else
        {
            break;
        }
    }
    while(true);

    suspend();
}

}
}
