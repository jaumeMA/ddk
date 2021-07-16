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

template<typename Traits, typename Function>
class constrained_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
	typedef typename Traits::iterable_base_traits iterable_base_traits;

public:
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::action action;

	constrained_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef,const Function& i_constrain);

private:
	void iterate_impl(const function<action(reference)>& i_try,const shift_action& i_initialAction,action_state_lent_ptr i_actionStatePtr) override;
	void iterate_impl(const function<action(const_reference)>& i_try,const shift_action& i_initialAction,action_state_lent_ptr i_actionStatePtr) const override;

	iterable_impl_dist_ref<iterable_base_traits> m_iterableRef;
	const Function m_constrain;
};

}
}

#include "ddk_constrained_iterable_impl.inl"