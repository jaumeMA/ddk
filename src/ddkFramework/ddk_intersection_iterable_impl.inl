
namespace ddk
{
namespace detail
{

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
void intersection_iterable_impl<Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initState);
}
template<typename ... Iterables>
void intersection_iterable_impl<Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initState);
}
template<typename ... Iterables>
size_t intersection_iterable_impl<Iterables...>::size() const
{
    return 0;
}
template<typename ... Iterables>
bool intersection_iterable_impl<Iterables...>::empty() const
{
    return true;
}
template<typename ... Iterables>
template<size_t ... Indexs>
void intersection_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    tuple<awaitable<typename Iterables::reference>...> awaitableTuple(await(make_function(this,&intersection_iterable_impl<Iterables...>::private_iterate_impl<Indexs>,m_iterables.template get<Indexs>(),i_initState))...);
    tuple<awaited_result<typename Iterables::reference>...> awaitableResultTuple;

    bool cond = true;
    do
    {
        (awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...);

        if((static_cast<bool>(awaitableResultTuple.template get<Indexs>()) && ...))
        {
            eval(i_try,make_tuple(awaitableResultTuple.template get<Indexs>().get() ...));
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
void intersection_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    tuple<awaitable<typename Iterables::const_reference>...> awaitableTuple(await(make_function(this,&intersection_iterable_impl<Iterables...>::private_iterate_impl<Indexs>,m_iterables.template get<Indexs>(),i_initState))...);
    tuple<awaited_result<typename Iterables::const_reference>...> awaitableResultTuple;

    bool cond = true;
    do
    {
        (awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...);

        if((static_cast<bool>(awaitableResultTuple.template get<Indexs>()) && ...))
        {
            eval(i_try,make_tuple(awaitableResultTuple.template get<Indexs>().get() ...));
        }
        else
        {
            break;
        }
    }
    while(true);
}
template<typename ... Iterables>
template<size_t Index>
typename mpl::nth_type_of<Index,Iterables...>::type::reference intersection_iterable_impl<Iterables...>::private_iterate_impl(typename mpl::nth_type_of<Index,Iterables...>::type& i_iterable, const iter::iterable_state& i_initState)
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type::reference curr_reference;
    typedef typename mpl::nth_type_of<Index,Iterables...>::type::iterable_value curr_iterable_value;

    i_iterable.iterate(make_function([](curr_iterable_value i_value)
                                     {
                                         yield<curr_reference>(*i_value);
                                     }),nullptr,i_initState);

    suspend();
}
template<typename ... Iterables>
template<size_t Index>
typename mpl::nth_type_of<Index,Iterables...>::type::const_reference intersection_iterable_impl<Iterables...>::private_iterate_impl(const typename mpl::nth_type_of<Index,Iterables...>::type& i_iterable, const iter::iterable_state& i_initState) const
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type::const_reference curr_const_reference;
    typedef typename mpl::nth_type_of<Index,Iterables...>::type::iterable_const_value curr_iterable_const_value;

    i_iterable.iterate(make_function([](curr_iterable_const_value i_value){ yield<curr_const_reference>(*i_value); }),nullptr,i_initState);

    suspend();
}

}
}
