
namespace ddk
{
namespace detail
{

template<typename Callable, typename ... NestedCallables>
union_function<Callable,NestedCallables...>::union_function(const Callable& i_callableTransform, const NestedCallables& ... i_args)
: m_returnTransform(i_callableTransform)
, m_nestedCallables(i_args ...)
{
}
template<typename Callable, typename ... NestedCallables>
template<typename ... Args>
typename union_function<Callable,NestedCallables...>::callable_return_type union_function<Callable,NestedCallables...>::operator()(Args&& ... i_args) const
{
    return execute(typename mpl::make_sequence<0,s_num_callables>::type{}, std::forward<Args>(i_args)...);
}
template<typename Callable, typename ... NestedCallables>
template<int ... Indexs, typename ... Args>
typename union_function<Callable,NestedCallables...>::callable_return_type union_function<Callable,NestedCallables...>::execute(const mpl::sequence<Indexs...>&, Args&& ... i_args) const
{
	return m_returnTransform.eval(m_nestedCallables.template get<Indexs>().eval(std::forward<Args>(i_args) ...) ...);
}

}
}
