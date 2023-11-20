
#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename T>
iterable_order<T>::iterable_order(const T& i_order)
: m_order(i_order)
{
}
template<typename T>
template<typename Iterable>
T iterable_order<T>::init(const Iterable& i_iterable) const
{
	return m_order.init(i_iterable);
}

template<typename Iterable,typename ActionResolver>
TEMPLATE(typename IIterable,typename AActionResolver)
REQUIRED(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(ActionResolver,AActionResolver))
ordered_iterable_impl<Iterable,ActionResolver>::ordered_iterable_impl(IIterable&& i_iterable,AActionResolver&& i_actionResolver)
: m_iterable(std::forward<IIterable>(i_iterable))
, m_actionResolver(std::forward<AActionResolver>(i_actionResolver))
{
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,reference))
void ordered_iterable_impl<Iterable,ActionResolver>::iterate_impl(Function&& i_try,const Action& i_initialAction)
{
	m_iterable.iterate_impl([&](reference i_value)
	{ 
		return m_actionResolver(ddk::eval(std::forward<Function>(i_try),i_value));
	},m_actionResolver(i_initialAction));
}
template<typename Iterable,typename ActionResolver>
TEMPLATE(typename Function,typename Action)
REQUIRED(IS_CALLABLE_BY(Function,const_reference))
void ordered_iterable_impl<Iterable,ActionResolver>::iterate_impl(Function&& i_try,const Action& i_initialAction) const
{
	m_iterable.iterate_impl([&](const_reference i_value)
	{
		return m_actionResolver(ddk::eval(std::forward<Function>(i_try),i_value));
	},m_actionResolver(i_initialAction));
}

}
}
