#pragma once

#include "ddk_fiber_defs.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_ucontext.h"
#include "ddk_fiber_local.h"
#include "ddk_stack_allocator.h"
#include "ddk_thread_yielder.h"
#include "ddk_fiber_scheduler_interface.h"

namespace ddk
{
namespace detail
{

struct this_fiber_t
{
public:
	this_fiber_t();
	this_fiber_t(stack_alloc_shared_ref i_stackAlloc, size_t i_maxNumPages);

	ucontext_t* get_context();
	const ucontext_t* get_context() const;
	fiber_id get_id() const;
	stack_allocator get_allocator() const;

private:
	mutable ucontext_t m_context;
	stack_alloc_shared_ref m_stackAllocImpl;
	size_t m_numMaxPages = 0;
};

struct fiber_impl
{
public:
	fiber_impl();
	fiber_impl(stack_allocator i_stackAlloc);
	fiber_impl(const fiber_impl&) = delete;
	fiber_impl(fiber_impl&& other);
	~fiber_impl();
	template<typename Return>
	void start_from(this_fiber_t& other, const std::function<Return()>& i_function);
	void start(const std::function<void()>& i_function);
	void stop();
	yielder_context* resume_from(this_fiber_t& other);
	void resume_to(this_fiber_t& other, yielder_context* i_context = nullptr);
	fiber_id get_id() const;
	void set_state(FiberExecutionState i_state);
	FiberExecutionState get_state() const;
	ucontext_t* get_context() const;
	void set_executor(fiber_scheduler_interface_lent_ptr i_executor);
	bool joinable() const;
	bool operator==(const fiber_impl& other) const;

private:
	fiber_id m_id;
	fiber_scheduler_interface_lent_ptr m_executor;
	mutable ucontext_t m_context;
	FiberExecutionState m_state;
	stack_allocator m_alloc;
};

typedef unique_reference_wrapper<fiber_impl> fiber_impl_unique_ref;
typedef unique_reference_wrapper<const fiber_impl> fiber_impl_const_unique_ref;
typedef unique_pointer_wrapper<fiber_impl> fiber_impl_unique_ptr;
typedef unique_pointer_wrapper<const fiber_impl> fiber_impl_const_unique_ptr;

typedef lent_reference_wrapper<fiber_impl> fiber_impl_lent_ref;
typedef lent_reference_wrapper<const fiber_impl> fiber_impl_const_lent_ref;
typedef lent_pointer_wrapper<fiber_impl> fiber_impl_lent_ptr;
typedef lent_pointer_wrapper<const fiber_impl> fiber_impl_const_lent_ptr;

}

}

#include "ddk_fiber_impl.inl"
