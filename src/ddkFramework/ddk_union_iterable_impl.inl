
namespace ddk
{
namespace detail
{

template<size_t Index, typename ... Iterables>
typename union_iterable_impl<Iterables...>::action navigate(union_iterable_impl<Iterables...>& i_iterable, const function<typename union_iterable_impl<Iterables...>::action(typename union_iterable_impl<Iterables...>::reference)>& i_try, const iter::shift_action& i_initialAction)
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::reference reference;

    typename union_iterable_impl<Iterables...>::action lastAction;

    i_iterable.m_iterables.template get<Index>().iterate(make_function([i_try,&lastAction,&i_iterable](reference i_value)
	{
		lastAction = eval(i_try,i_value);
		i_iterable.m_iterables.template get<Index>().forward_action(lastAction);
	}),i_initialAction);


	return lastAction;
}
template<size_t Index, typename ... Iterables>
typename union_iterable_impl<Iterables...>::action navigate(const union_iterable_impl<Iterables...>& i_iterable, const function<typename union_iterable_impl<Iterables...>::action(typename union_iterable_impl<Iterables...>::const_reference)>& i_try, const iter::shift_action& i_initialAction)
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::const_reference const_reference;

    typename union_iterable_impl<Iterables...>::action lastAction;

    i_iterable.m_iterables.template get<Index>().iterate(make_function([i_try,&lastAction,&i_iterable](const_reference i_value)
	{
		lastAction = eval(i_try,i_value);
		i_iterable.m_iterables.template get<Index>().forward_action(lastAction);
	}),i_initialAction);

	return lastAction;
}

template<size_t ... Indexs, typename ... Iterables>
const typename union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::size_t_func union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::s_size_funcs[s_num_iterables] = { &union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::template iterable_size<Indexs> ...};

template<size_t ... Indexs, typename ... Iterables>
union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::union_iterable_visitor_type(size_t i_currIterableIndex, const tuple<Iterables...>& i_iterables)
: m_currIterableIndex(i_currIterableIndex)
, m_iterables(i_iterables)
{
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::stop_action& i_action) const
{
    suspend();

	return crash_on_return<std::pair<size_t,iter::shift_action>>::value();
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::erase_action& i_action) const
{
	suspend();

	return crash_on_return<std::pair<size_t,iter::shift_action>>::value();
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::add_action& i_action) const
{
    suspend();

	return crash_on_return<std::pair<size_t,iter::shift_action>>::value();
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::go_forward_action& i_action) const
{
    const size_t newIterableIndex = m_currIterableIndex + 1;
    if(newIterableIndex < s_num_iterables)
    {
        return std::make_pair(newIterableIndex,iter::go_to_place);
    }
    else
    {
        return std::make_pair(iter::iterable_state::npos,iter::go_to_place);
    }
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::go_backward_action& i_action) const
{
    if(m_currIterableIndex > 0)
    {
        return std::make_pair(m_currIterableIndex-1,iter::go_to_place(static_cast<int>((*s_size_funcs[m_currIterableIndex-1])(m_iterables))));
    }
    else
    {
        return std::make_pair(iter::iterable_state::npos,iter::go_to_place);
    }
}
template<size_t ... Indexs, typename ... Iterables>
std::pair<size_t,iter::shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::visit(const iter::shift_action& i_action) const
{
    return std::make_pair(iter::iterable_state::npos,iter::go_to_place);
}
template<size_t ... Indexs, typename ... Iterables>
template<size_t Index>
size_t union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::iterable_size(const tuple<Iterables...>& i_iterables)
{
    return i_iterables.template get<Index>().size();
}

template<typename ... Iterables>
union_iterable_impl<Iterables...>::union_iterable_impl(const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
{
}
template<typename ... Iterables>
union_iterable_impl<Iterables...>::union_iterable_impl(const tuple<Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
{
}
template<typename ... Iterables>
const tuple<Iterables...>& union_iterable_impl<Iterables...>::get_iterables() const
{
    return m_iterables;
}
template<typename ... Iterables>
tuple<Iterables...>& union_iterable_impl<Iterables...>::get_iterables()
{
    return m_iterables;
}
template<typename ... Iterables>
void union_iterable_impl<Iterables...>::iterate_impl(const function<action(reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
void union_iterable_impl<Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr)
{
    typedef action(*navigate_func)(union_iterable_impl<Iterables...>&, const function<action(reference)>&,const iter::shift_action&);

    static const navigate_func s_navFuncs[s_num_iterables] = { navigate<Indexs,Iterables...> ...};

    size_t iterableIndex= (i_initialAction.shifting() == -1) ? s_num_iterables - 1 : 0;
	iter::shift_action initialAction = i_initialAction;

    while(iterableIndex != iter::iterable_state::npos)
    {
        const action lastAction = (*s_navFuncs[iterableIndex])(*this,i_try,initialAction);

        union_iterable_visitor<Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,iter::shift_action> currIndexes = lastAction.visit(unionVisitor);

        iterableIndex = currIndexes.first;
		initialAction = currIndexes.second;
    }

    suspend();
}
template<typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const iter::shift_action& i_initialAction, iter::action_state_lent_ptr i_actionStatePtr) const
{
	typedef action(*navigate_func)(const union_iterable_impl<Iterables...>&, const function<action(const_reference)>&, const iter::shift_action&);

	static const navigate_func s_navFuncs[s_num_iterables] = { &navigate<Indexs,Iterables...> ... };

    size_t iterableIndex= 0;
	iter::shift_action initialAction = i_initialAction;

    while(iterableIndex != iter::iterable_state::npos)
    {
        const action lastAction = (*s_navFuncs[iterableIndex])(*this,i_try,initialAction);

        union_iterable_visitor<Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,iter::shift_action> currIndexes = lastAction.visit(unionVisitor);

        iterableIndex = currIndexes.first;
		initialAction = currIndexes.second;
    }

    suspend();
}
template<typename ... Iterables>
size_t union_iterable_impl<Iterables...>::size() const
{
	TODO("Pending");

    return 0;
}
template<typename ... Iterables>
bool union_iterable_impl<Iterables...>::empty() const
{
	TODO("Pending");

	return true;
}

}
}
