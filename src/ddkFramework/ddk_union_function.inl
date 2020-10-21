
namespace ddk
{
namespace detail
{

template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(const Callable& i_callable, const Callables& ... i_callables)
: m_callables(i_callable,i_callables...)
{
}
template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(const tuple<Callable,Callables...>& i_callables)
: m_callables(i_callables)
{
}
template<typename Callable, typename ... Callables>
template<typename ... Args>
typename union_function<Callable,Callables...>::callable_return_type union_function<Callable,Callables...>::operator()(Args&& ... i_args) const
{
    return execute(typename mpl::make_sequence<0,s_num_callables>::type{}, tuple<Args...>{ std::forward<Args>(i_args)... } );
}
template<typename Callable, typename ... Callables>
template<size_t ... Indexs, typename ... Args>
typename union_function<Callable,Callables...>::callable_return_type union_function<Callable,Callables...>::execute(const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const
{
    typedef typename sequence_composer<0,typename mpl::aqcuire_callable_return_type<Callable>::args_type,typename mpl::aqcuire_callable_return_type<Callables>::args_type ...>::type composed_sequence;
    static const composed_sequence s_composedSequence;

    return callable_return_type{ execute(m_callables.template get<Indexs>(),s_composedSequence.template get<Indexs>(),i_args) ... };
}
template<typename Callable, typename ... Callables>
template<typename CCallable, size_t ... Indexs, typename ... Args>
typename mpl::aqcuire_callable_return_type<CCallable>::return_type union_function<Callable,Callables...>::execute(const CCallable& i_callable, const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const
{
	return eval(i_callable,i_args.template get<Indexs>() ...);
}

}
}
