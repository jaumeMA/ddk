#pragma once

#include "ddk_iterable_impl_interface.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_constrain
{
public:
	iterable_constrain(const Function& i_transform);

	Function get_constrain() const;

private:
	const Function m_constrain;
};
template<typename Function>
iterable_constrain(Function&&) -> iterable_constrain<mpl::remove_qualifiers<Function>>;

template<typename Iterable,typename Constrain>
class constrained_iterable_impl : public iterable_impl_interface<typename Iterable::traits>
{
	friend inline auto deduce_adaptor(const constrained_iterable_impl& i_iterableImpl)
	{
		return deduce_adaptor(i_iterableImpl.m_iterable);
	}

public:
	typedef typename Iterable::traits traits;
	typedef typename traits::value_type value_type;
	typedef typename traits::reference reference;
	typedef typename traits::const_reference const_reference;

	TEMPLATE(typename IIterable,typename CConstrain)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Constrain,CConstrain))
	constrained_iterable_impl(IIterable&& i_iterable,CConstrain&& i_constrain);

	TEMPLATE(typename Function,typename Action)
	REQUIRES(IS_CALLABLE_BY(Function,reference))
	iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction);
	TEMPLATE(typename Function,typename Action)
	REQUIRES(IS_CALLABLE_BY(Function,const_reference))
	iterable_result iterate_impl(Function&& i_try,const Action& i_initialAction) const;

private:
	Iterable m_iterable;
	const Constrain m_constrain;
};

}
}

#include "ddk_constrained_iterable_impl.inl"