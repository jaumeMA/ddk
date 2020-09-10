
namespace ddk
{
namespace detail
{

template<typename Callable, typename ... NestedCallables>
intersection_function<Callable,NestedCallables...>::intersection_function(const Callable& i_callableTransform, const NestedCallables& ... i_args)
: m_returnTransform(i_callableTransform)
, m_nestedCallables(i_args ...)
{
}
template<typename Callable, typename ... NestedCallables>
intersection_function<Callable,NestedCallables...>::intersection_function(const Callable& i_callableTransform, const tuple<NestedCallables...>& i_callables)
: m_returnTransform(i_callableTransform)
, m_nestedCallables(i_callables)
{
}
template<typename Callable, typename ... NestedCallables>
template<typename ... Args>
typename intersection_function<Callable,NestedCallables...>::callable_return_type intersection_function<Callable,NestedCallables...>::operator()(Args&& ... i_args) const
{
    return execute(typename mpl::make_sequence<0,s_num_callables>::type{}, mpl::forward<Args>(i_args)...);
}
template<typename Callable, typename ... NestedCallables>
template<int ... Indexs, typename ... Args>
typename intersection_function<Callable,NestedCallables...>::callable_return_type intersection_function<Callable,NestedCallables...>::execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const
{
	return m_returnTransform(m_nestedCallables.template get<Indexs>()(std::forward<Args>(i_args) ...) ...);
}

template<typename ... Callables>
intersected_functions<Callables...>::intersected_functions(const Callables& ... i_callables)
: m_nestedCallables(i_callables...)
{
}
template<typename ... Callables>
const tuple<Callables...>& intersected_functions<Callables...>::get_callables() const
{
	return m_nestedCallables;
}

}

template<typename Callable, typename ... Callables>
detail::intersection_function<Callable, Callables...> operator<=(const Callable& i_callable, const detail::intersected_functions<Callables...>& i_nestedCallable)
{
	return detail::intersection_function{i_callable,i_nestedCallable.get_callables()};
}

}