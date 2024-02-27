#pragma once

namespace ddk
{
namespace detail
{

template<typename T,typename TT>
struct embedded_private_async_state : private_async_state<T>
{
	static_assert(IS_LENDABLE_COND(TT),"You shall provide a lendable executor type");

public:
	embedded_private_async_state();
	~embedded_private_async_state();

	template<typename ... Args>
	TT& attach(Args&& ... i_args);
	void deallocate(TT* i_ptr) const;

private:
	typedef distributed_control_block<TT,deleter_proxy<embedded_private_async_state<T,TT>>> distributed_async_control_block;

	mutable typed_arena<TT> m_arena;
	distributed_async_control_block m_refCounter;
};

template<typename T,typename TT>
using embedded_private_async_state_shared_ref = shared_reference_wrapper<embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_const_shared_ref = shared_reference_wrapper<const embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_shared_ptr = shared_pointer_wrapper<embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_const_shared_ptr = shared_pointer_wrapper<const embedded_private_async_state<T,TT>>;

template<typename T,typename TT>
using embedded_private_async_state_weak_ptr = weak_pointer_wrapper<embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_const_weak_ptr = weak_pointer_wrapper<const embedded_private_async_state<T,TT>>;

template<typename T,typename TT>
using embedded_private_async_state_lent_ref = lent_reference_wrapper<embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_const_lent_ref = lent_reference_wrapper<const embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_lent_ptr = lent_pointer_wrapper<embedded_private_async_state<T,TT>>;
template<typename T,typename TT>
using embedded_private_async_state_const_lent_ptr = lent_pointer_wrapper<const embedded_private_async_state<T,TT>>;

}
}

#include "ddk_embedded_async_shared_state.inl"