
namespace ddk
{
namespace detail
{

template<typename Iterable, typename ... Iterables>
intersection_iterable_impl<Iterable,Iterables...>::intersection_iterable_impl(const Iterable& i_iterable, const Iterables& ... i_iterables)
: m_iterables(i_iterable,i_iterables...)
{
}
template<typename Iterable, typename ... Iterables>
intersection_iterable_impl<Iterable,Iterables...>::intersection_iterable_impl(const tuple<Iterable,Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
{
}
template<typename Iterable, typename ... Iterables>
const tuple<Iterable,Iterables...>& intersection_iterable_impl<Iterable,Iterables...>::get_iterables() const
{
    return m_iterables;
}
template<typename Iterable, typename ... Iterables>
tuple<Iterable,Iterables...>& intersection_iterable_impl<Iterable,Iterables...>::get_iterables()
{
    return m_iterables;
}
template<typename Iterable, typename ... Iterables>
void intersection_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    m_iterables.template get<0>().iterate(make_function(this,&intersection_iterable_impl<Iterable,Iterables...>::template iterate_impl<0>,i_try,i_initState,arg_0),nullptr,i_initState);
}
template<typename Iterable, typename ... Iterables>
void intersection_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    m_iterables.template get<0>().iterate(make_function(this,&intersection_iterable_impl<Iterable,Iterables...>::template iterate_impl<0>,i_try,i_initState,arg_0),nullptr,i_initState);
}
template<typename Iterable, typename ... Iterables>
size_t intersection_iterable_impl<Iterable,Iterables...>::size() const
{
    return 0;
}
template<typename Iterable, typename ... Iterables>
bool intersection_iterable_impl<Iterable,Iterables...>::empty() const
{
    return true;
}
template<typename Iterable, typename ... Iterables>
template<size_t Index, typename ... References>
void intersection_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState, typename mpl::nth_type_of<Index,Iterable,Iterables...>::type::iterable_value i_iterableValue, References ... i_references)
{
    if constexpr (Index < s_num_iterables - 1)
    {
        typedef typename mpl::nth_type_of<Index,Iterable,Iterables...>::type curr_iterable;
        typedef typename mpl::nth_type_of<Index+1,Iterable,Iterables...>::type next_iterable;
        typedef typename curr_iterable::reference reference;

        m_iterables.template get<Index+1>().iterate(make_function(this,&intersection_iterable_impl<Iterable,Iterables...>::template iterate_impl<Index+1,References...,embedded_type<reference>>,i_try,i_initState,arg_0,i_references...,embedded_type<reference>(*i_iterableValue)),nullptr,i_initState);
    }
    else
    {
        eval(i_try,make_tuple(*i_references...,*i_iterableValue));
    }
}
template<typename Iterable, typename ... Iterables>
template<size_t Index, typename ... References>
void intersection_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState, typename mpl::nth_type_of<Index,Iterable,Iterables...>::type::iterable_const_value i_iterableValue, References ... i_references) const
{
    if constexpr (Index < s_num_iterables - 1)
    {
        typedef typename mpl::nth_type_of<Index,Iterable,Iterables...>::type curr_iterable;
        typedef typename mpl::nth_type_of<Index+1,Iterable,Iterables...>::type next_iterable;
        typedef typename curr_iterable::const_reference const_reference;

        m_iterables.template get<Index+1>().iterate(make_function(this,&intersection_iterable_impl<Iterable,Iterables...>::template iterate_impl<Index+1,References...,embedded_type<const_reference>>,i_try,i_initState,arg_0,i_references...,embedded_type<const_reference>(*i_iterableValue)),nullptr,i_initState);
    }
    else
    {
        eval(i_try,make_tuple(*i_references...,*i_iterableValue));
    }
}

}
}
