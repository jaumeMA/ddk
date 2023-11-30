
namespace ddk
{
namespace detail
{

template<typename Function>
iterable_constrain<Function>::iterable_constrain(const Function& i_constrain)
: m_constrain(i_constrain)
{
}
template<typename Function>
Function iterable_constrain<Function>::get_constrain() const
{
	return m_constrain;
}

template<typename Iterable,typename Constrain>
TEMPLATE(typename IIterable,typename CConstrain)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Constrain,CConstrain))
constrained_iterable_impl<Iterable,Constrain>::constrained_iterable_impl(IIterable&& i_iterable,CConstrain&& i_constrain)
: m_iterable(std::forward<IIterable>(i_iterable))
, m_constrain(std::forward<CConstrain>(i_constrain))
{
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
iterable_result constrained_iterable_impl<Iterable,Constrain>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
    typedef typename mpl::aqcuire_callable_return_type<Function>::type return_type;

    return m_iterable.iterate_impl([&](reference i_value) -> return_type { if (ddk::eval(m_constrain,i_value)) { return ddk::eval(i_try,i_value); } else { return return_type{ stop_iteration }; }},i_initialAction);
}
template<typename Iterable,typename Constrain>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,const_reference))
iterable_result constrained_iterable_impl<Iterable,Constrain>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
    typedef typename mpl::aqcuire_callable_return_type<Function>::type return_type;

    return m_iterable.iterate_impl([&](reference i_value) -> return_type { if (ddk::eval(m_constrain,i_value)) { return ddk::eval(i_try,i_value); } else { return return_type{ stop_iteration }; }},i_initialAction);

}

}
}