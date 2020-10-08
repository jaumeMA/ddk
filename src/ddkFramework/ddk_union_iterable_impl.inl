#pragma once

namespace ddk
{
namespace detail
{

template<size_t ... Indexs, typename ... Iterables>
const typename union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::size_t_func union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::s_size_funcs[s_num_iterables] = { &union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::template iterable_size<Indexs> ...};

template<size_t ... Indexs, typename ... Iterables>
union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::union_iterable_visitor_type(size_t i_currIterableIndex, const tuple<Iterables...>& i_iterables)
: m_currIterableIndex(i_currIterableIndex)
, m_iterables(i_iterables)
{
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::stop_action& i_action) const
{
    suspend();
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::erase_action& i_action) const
{
    if(m_currIterableIndex < s_num_iterables)
    {
        return std::make_pair(m_currIterableIndex+1,0);
    }
    else
    {
        return std::make_pair(iter::iterable_state::npos,iter::iterable_state::npos);
    }
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::add_action& i_action) const
{
    suspend();
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::go_forward_action& i_action) const
{
    if(m_currIterableIndex < s_num_iterables)
    {
        return std::make_pair(m_currIterableIndex+1,0);
    }
    else
    {
        return std::make_pair(iter::iterable_state::npos,iter::iterable_state::npos);
    }
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::go_backward_action& i_action) const
{
    if(m_currIterableIndex > 0)
    {
        return std::make_pair(m_currIterableIndex-1,(*s_size_funcs[m_currIterableIndex-1])(m_iterables));
    }
    else
    {
        return std::make_pair(iter::iterable_state::npos,iter::iterable_state::npos);
    }
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,size_t> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::shift_action& i_action) const
{
    return std::make_pair(iter::iterable_state::npos,iter::iterable_state::npos);
}
template<size_t ... Indexs, typename ... Iterables>
template<size_t Index>
size_t union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::iterable_size(const tuple<Iterables...>& i_iterables)
{
    return i_iterables.template get<Index>().size();
}

template<typename Iterable, typename ... Iterables>
union_iterable_impl<Iterable,Iterables...>::union_iterable_impl(const Iterable& i_iterable, const Iterables& ... i_iterables)
: m_iterables(i_iterable,i_iterables...)
{
}
template<typename Iterable, typename ... Iterables>
void union_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initState);
}
template<typename Iterable, typename ... Iterables>
void union_iterable_impl<Iterable,Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initState);
}
template<typename Iterable, typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterable,Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    typedef action(*navigate_func)(tuple<Iterable,Iterables...>&,const function<action(reference)>&,const iter::iterable_state&);

    static const navigate_func s_navFuncs[s_num_iterables] = { &union_iterable_impl<Iterable,Iterables...>::template navigate<Indexs> ...};

    size_t iterableIndex= 0;
    iter::iterable_state iterableInitState = i_initState;

    while(iterableIndex != iter::iterable_state::npos)
    {
        const action lastAction = (*s_navFuncs[iterableIndex])(m_iterables,i_try,iterableInitState);

        union_iterable_visitor<Iterable,Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,size_t> currIndexes = lastAction.visit(unionVisitor);

        iterableIndex = currIndexes.first;
        iterableInitState = iter::iterable_state(currIndexes.second);
    }
}
template<typename Iterable, typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterable,Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState) const
{
    typedef action(*navigate_func)(const tuple<Iterable,Iterables...>&,const function<action(const_reference)>&,const iter::iterable_state&);

    static const navigate_func s_navFuncs[s_num_iterables] = { &union_iterable_impl<Iterable,Iterables...>::template navigate<Indexs> ...};

    size_t iterableIndex= 0;
    iter::iterable_state iterableInitState = i_initState;

    while(iterableIndex != iter::iterable_state::npos)
    {
        const action lastAction = (*s_navFuncs[iterableIndex])(m_iterables,i_try,iterableInitState);

        union_iterable_visitor<Iterable,Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,size_t> currIndexes = lastAction.visit(unionVisitor);

        iterableIndex = currIndexes.first;
        iterableInitState = iter::iterable_state(currIndexes.second);
    }
}
template<typename Iterable, typename ... Iterables>
size_t union_iterable_impl<Iterable,Iterables...>::size() const
{
    size_t res = 0;

    return res;
}
template<typename Iterable, typename ... Iterables>
bool union_iterable_impl<Iterable,Iterables...>::empty() const
{
    return true;
}
template<typename Iterable, typename ... Iterables>
template<size_t Index>
typename union_iterable_impl<Iterable,Iterables...>::action union_iterable_impl<Iterable,Iterables...>::navigate(tuple<Iterable,Iterables...>& i_tuple, const function<action(reference)>& i_try, const iter::iterable_state& i_initState)
{
    typedef typename mpl::nth_type_of<Index,Iterable,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::iterable_value iterable_value;

    action lastAction;

    i_tuple.template get<Index>().iterate(make_function([&i_try,&lastAction](iterable_value i_value) { lastAction = eval(i_try,*i_value); }),nullptr,i_initState);

    return lastAction;
}
template<typename Iterable, typename ... Iterables>
template<size_t Index>
typename union_iterable_impl<Iterable,Iterables...>::action union_iterable_impl<Iterable,Iterables...>::navigate(const tuple<Iterable,Iterables...>& i_tuple, const function<action(const_reference)>& i_try, const iter::iterable_state& i_initState)
{
    typedef typename mpl::nth_type_of<Index,Iterable,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::iterable_const_value iterable_const_value;

    action lastAction;

    i_tuple.template get<Index>().iterate(make_function([&i_try,&lastAction](iterable_const_value i_value) { lastAction = eval(i_try,*i_value); }),nullptr,i_initState);

    return lastAction;
}

}
}