#include "ddk_stack_allocator.h"
#include "ddk_dynamic_stack_allocator.h"
#include "ddk_reference_wrapper.h"

#if defined(WIN32)

#define CALL_LAST 0
#define CALL_FIRST 1

namespace ddk
{

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
	{
		if(std::pair<void*,void*>*& fiberArena = ddk::stack_allocator::get_curr_arena())
		{
			//check that stack pointer is inside our scope
			if(const stack_allocator_interface* currAllocImpl = stack_allocator::get_curr_alloc_impl())
			{
				if(currAllocImpl->reallocate(*fiberArena,reinterpret_cast<void*>(pExceptionInfo->ContextRecord->Rsp)))
				{
					//every time we receive an exception under a current fiber arena, reset stack limits
					set_curr_thread_stack_limit(fiberArena->second);

					return EXCEPTION_CONTINUE_EXECUTION;
				}
				else
				{
					return EXCEPTION_EXECUTE_HANDLER;
				}
			}
			else
			{
				return EXCEPTION_EXECUTE_HANDLER;
			}
		}
		else
		{
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

}

#elif defined(__LINUX__) || defined(__APPLE__)

#include <signal.h>

namespace ddk
{

void segfault_sigaction(int i_code,siginfo_t* i_sigInfo,void* i_context);

}

void segfault_sigaction_bridge(int i_code,siginfo_t* i_sigInfo,void* i_context)
{
    ddk::segfault_sigaction(i_code,i_sigInfo,i_context);
}
void ddk::segfault_sigaction(int i_code,siginfo_t* i_sigInfo,void* i_context)
{
    int tmp = 0;
    if(std::pair<void*,void*>*& fiberArena = ddk::stack_allocator::get_curr_arena())
    {
		if(const stack_allocator_interface* currAllocImpl = stack_allocator::get_curr_alloc_impl())
		{
			if(currAllocImpl->reallocate(*fiberArena,i_sigInfo->si_addr))
			{
				//every time we receive an exception under a current fiber arena, reset stack limits
				set_curr_thread_stack_limit(fiberArena->second);
			}
			else
			{
				DDK_FAIL_OR_LOG("Segfault outside stack scope");

				exit(0);
			}
		}
        else
        {
            DDK_FAIL_OR_LOG("Segfault outside stack scope");

            exit(0);
        }
    }
    else
    {
        DDK_FAIL_OR_LOG("Received seg fault with no associated arena");

        exit(0);
    }
}

#endif

namespace ddk
{

bool initialize_thread_stack()
{

#if defined(WIN32)

	AddVectoredExceptionHandler(1,VectoredExceptionHandler);

#elif defined(__LINUX__) or defined(__APPLE__)

    static char signalStackArena[SIGSTKSZ];
    static struct sigaction sa;

	stack_t signalStack;
	signalStack.ss_sp = signalStackArena;
    signalStack.ss_size = SIGSTKSZ;
    signalStack.ss_flags = 0;
    int val = sigaltstack(&signalStack,nullptr);

    sa.sa_sigaction = segfault_sigaction_bridge;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK | SA_SIGINFO;

	sigaction(SIGSEGV,&sa,nullptr);

#endif

	return true;
}
std::unordered_map<fiber_id,std::pair<void*,void*>>* initialize_thread_map()
{
	static thread_local std::unordered_map<fiber_id,std::pair<void*,void*>> fiberStackMap;

	return &fiberStackMap;
}

#if defined(WIN32)

size_t getpagesize()
{
	SYSTEM_INFO sSysInfo;

	GetSystemInfo(&sSysInfo);

	return sSysInfo.dwPageSize;
}

#endif

size_t stack_allocator_interface::s_pageSize = getpagesize();

stack_allocator::stack_allocator(stack_alloc_const_shared_ref i_stackAllocImpl)
: m_stackAllocImpl(i_stackAllocImpl)
{
}
stack_allocator::stack_allocator(stack_allocator&& other)
: m_stackAllocImpl(other.m_stackAllocImpl)
{
}
std::pair<size_t,void*> stack_allocator::allocate(fiber_id i_id) const
{
	static const thread_local bool _ = initialize_thread_stack();

	m_arena = initialize_thread_map();

	std::pair<void*,void*>& fiberAlloc = (*m_arena)[i_id];
	fiberAlloc.first = m_stackAllocImpl->reserve(stack_allocator_interface::k_maxNumStackPages);
	fiberAlloc.second = m_stackAllocImpl->allocate(fiberAlloc.first,1);

	return std::make_pair(stack_allocator_interface::s_pageSize,fiberAlloc.second);
}
void* stack_allocator::attach(fiber_id i_id)
{
	DDK_ASSERT(m_arena, "Attaching with no fiber map set");

	const stack_allocator_interface*& currAllocImpl = get_curr_alloc_impl();
	currAllocImpl = m_stackAllocImpl.get();

	std::pair<void*,void*>*& currArena = get_curr_arena();
	currArena = &(*m_arena)[i_id];

	return currArena->second;
}
void stack_allocator::detach()
{
	std::pair<void*,void*>*& currArena = get_curr_arena();

	currArena = nullptr;
}
void stack_allocator::deallocate(fiber_id i_id) const
{
	DDK_ASSERT(m_arena, "Deallocating with no fiber map set");

	std::pair<void*,void*>& fiberAlloc = (*m_arena)[i_id];

	m_stackAllocImpl->deallocate(fiberAlloc.first,(reinterpret_cast<char*>(fiberAlloc.first) - reinterpret_cast<char*>(fiberAlloc.second)) / stack_allocator_interface::s_pageSize);
	m_stackAllocImpl->release(fiberAlloc.first,stack_allocator_interface::k_maxNumStackPages);

	m_arena->erase(i_id);
}
std::pair<void*,void*>*& stack_allocator::get_curr_arena()
{
	static thread_local std::pair<void*,void*>* currArena = nullptr;

	return currArena;
}
const stack_allocator_interface*& stack_allocator::get_curr_alloc_impl()
{
	static thread_local const stack_allocator_interface* currStackAllocImpl = nullptr;

	return currStackAllocImpl;
}

}
