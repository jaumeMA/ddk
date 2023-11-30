#pragma once

namespace ddk
{
namespace detail
{

template<typename T>
class iterable_order
{
public:
	iterable_order(const T& i_order);

	const T& order() const;

private:
	const T m_order;
};

template<typename Iterable, typename ActionResolver>
class ordered_iterable_impl : public iterable_impl_interface<typename Iterable::traits>
{
	friend inline auto deduce_adaptor(const ordered_iterable_impl& i_iterableImpl)
	{
		return deduce_adaptor(i_iterableImpl.m_iterable);
	}

public:
	typedef typename Iterable::traits traits;
	typedef typename traits::value_type value_type;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

	TEMPLATE(typename IIterable,typename AActionResolver)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(ActionResolver,AActionResolver))
	ordered_iterable_impl(IIterable&& i_iterable, AActionResolver&& i_actionResolver);

	TEMPLATE(typename Function,typename Action)
	REQUIRES(IS_CALLABLE_BY(Function,reference))
	iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction);
	TEMPLATE(typename Function,typename Action)
	REQUIRES(IS_CALLABLE_BY(Function,const_reference))
	iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction) const;

private:
	Iterable m_iterable;
	const ActionResolver m_actionResolver;
};

}
}

#include "ddk_ordered_iterable_impl.inl"
