#pragma once

#include <cstddef>
#include <utility>
#include "ewas_thread_local.h"
#include <unordered_map>
#include "ewas_macros.h"
#include "ewas_stack_allocator_interface.h"
#include "ewas_fiber_defs.h"

#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

#endif

namespace ewas
{

class stack_allocator
{
#if defined(WIN32)

	friend LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo);

#elif defined(__LINUX__) or defined(__APPLE__)

	friend void segfault_sigaction(int i_code,siginfo_t* i_sigInfo,void* i_context);

#endif

public:
    typedef stack_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

	stack_allocator(stack_alloc_const_shared_ref i_stackAllocImpl);
	stack_allocator(const stack_allocator&) = delete;
	stack_allocator(stack_allocator&& other);
    std::pair<size_t,void*> allocate(fiber_id i_id) const;
	void* attach(fiber_id i_id);
	void detach();
    void deallocate(fiber_id i_id) const;
	stack_allocator& operator=(const stack_allocator&) = delete;
	stack_allocator& operator=(stack_allocator&&) = delete;

private:
	static std::pair<void*,void*>*& get_curr_arena();
	static const stack_allocator_interface*& get_curr_alloc_impl();

	stack_alloc_const_shared_ref m_stackAllocImpl;
	mutable std::unordered_map<fiber_id,std::pair<void*,void*>>* m_arena;
};

}
