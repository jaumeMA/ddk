#pragma once

#include "thread_utils.h"
#include "CriticalCallContext.h"
#include "ddk_intrusive_stack.h"
#include "ddk_intrusive_node.h"
#include "ddk_thread_local.h"

template<typename Traits>
class IAccessCriticalCallContext : protected Traits::provider_interface
{
	typedef ewas::lent_reference_wrapper<typename Traits::critical_context> critical_context_ref;
	typedef ewas::lent_reference_wrapper<const typename Traits::critical_context> critical_context_const_ref;
	typedef ewas::intrusive_stack<critical_context_ref> context_intrusive_stack;

public:
	typedef typename Traits::provider_interface provider_interface;
	typedef typename Traits::critical_context critical_context;
	typedef ewas::detail::intrusive_node<critical_context_ref> context_node_base;
	typedef CriticalCallContext<IAccessCriticalCallContext<Traits>> critical_call_context;
	typedef ConstCriticalCallContext<IAccessCriticalCallContext<Traits>> const_critical_call_context;

	inline const_critical_call_context create_call_context(const critical_context_ref& i_context) const
	{
		return const_critical_call_context(*this,i_context);
	}
	inline critical_call_context create_call_context(const critical_context_ref& i_context)
	{
		return critical_call_context(*this,i_context);
	}
	critical_context& acquire_call_context()
	{
		return *(get_context_stack().get_front_value());
	}
	const critical_context& acquire_call_context() const
	{
		return *(get_context_stack().get_front_value());
	}
	provider_interface& get_provider()
	{
		return *static_cast<provider_interface*>(this);
	}
	const provider_interface& get_provider() const
	{
		return *static_cast<const provider_interface*>(this);
	}
	inline void push_call_context(ewas::lent_reference_wrapper<context_node_base> i_node) const
	{
		get_context_stack().push(std::move(i_node));
	}
	inline void pop_call_context() const
	{
		get_context_stack().pop();
	}

private:
	static context_intrusive_stack& get_context_stack()
	{
		static ewas::threadlocal<context_intrusive_stack,Traits> s_contextStack;

		return s_contextStack.acquire(context_intrusive_stack::Lifo);
	}
};