
namespace ddk
{
namespace detail
{

template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const Callable& i_arg, const Callables& ... i_callables)
: m_callables(i_arg,i_callables...)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const tuple<Callable,Callables...>& i_callables)
: m_callables(i_callables)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(tuple<Callable,Callables...>&& i_callables)
: m_callables(std::move(i_callables))
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const intersection_function<Callable,Callables...>& other)
: m_callables(other.m_callables)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(intersection_function<Callable,Callables...>&& other)
: m_callables(std::move(other.m_callables))
{
}
template<typename Callable, typename ... Callables>
template<typename ... Args>
typename intersection_function<Callable,Callables...>::callable_return_type intersection_function<Callable,Callables...>::operator()(Args&& ... i_args) const
{
    return execute(typename mpl::make_sequence<0,s_num_callables>::type{}, std::forward<Args>(i_args)...);
}
template<typename Callable, typename ... Callables>
template<size_t ... Indexs, typename ... Args>
typename intersection_function<Callable,Callables...>::callable_return_type intersection_function<Callable,Callables...>::execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const
{
	return { eval(m_callables.template get<Indexs>(),std::forward<Args>(i_args) ...) ... };
}

}
}
