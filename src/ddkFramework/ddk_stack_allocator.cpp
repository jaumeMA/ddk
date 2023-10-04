#include "ddk_stack_allocator.h"
#include "ddk_dynamic_stack_allocator.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_utils.h"
#include "ddk_execution_context.h"

#if defined(WIN32)

#define CALL_LAST 0
#define CALL_FIRST 1

extern "C"
{
	void set_curr_thread_stack(void*);
}

namespace ddk
{

LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION ||
	   pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_STACK_OVERFLOW)
	{
		if(ddk::detail::execution_context* currFiberContext = get_current_execution_context())
		{
			detail::execution_stack& currStack = currFiberContext->get_stack();

			//check that stack pointer is inside our scope
			if(stack_alloc_const_lent_ptr& currAllocImpl = currStack.get_allocator())
			{
				if(currAllocImpl->reallocate(currStack,reinterpret_cast<void*>(pExceptionInfo->ExceptionRecord->ExceptionInformation[1])))
				{
					//every time we receive an exception under a current fiber arena, reset stack limits
					set_curr_thread_stack(&currStack);

					return EXCEPTION_CONTINUE_EXECUTION;
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
		else
		{
			return EXCEPTION_CONTINUE_SEARCH;
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
	if(ddk::detail::execution_context* currFiberContext = get_current_execution_context())
	{
        detail::execution_stack& currStack = currFiberContext->get_stack();

        if(stack_alloc_const_lent_ptr& currAllocImpl = currStack.get_allocator())
        {
            if(currAllocImpl->reallocate(currStack,i_sigInfo->si_addr))
            {
                //every time we receive an exception under a current fiber arena, reset stack limits
                set_curr_thread_stack(&currStack);
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

#if defined(WIN32)

size_t getpagesize()
{
	SYSTEM_INFO sSysInfo;

	GetSystemInfo(&sSysInfo);

	return sSysInfo.dwPageSize;
}

#endif

size_t stack_allocator_interface::s_pageSize = getpagesize();

stack_allocator::stack_allocator(stack_alloc_const_dist_ref i_stackAllocImpl, size_t i_numMaxPages)
: m_stackAllocImpl(i_stackAllocImpl)
, m_numMaxPages(i_numMaxPages)
{
}
stack_allocator::stack_allocator(stack_allocator&& other)
: m_stackAllocImpl(std::move(other.m_stackAllocImpl))
, m_numMaxPages(other.m_numMaxPages)
{
}
stack_allocator::~stack_allocator()
{
}
detail::execution_stack stack_allocator::allocate() const
{
	static const thread_local bool _ = initialize_thread_stack();

	void* initStack = m_stackAllocImpl->reserve(m_numMaxPages);
	std::pair<void*,void*> endDeallocStack = m_stackAllocImpl->allocate(initStack,s_num_ready_to_use_pages);

	return { initStack,endDeallocStack.first,reinterpret_cast<char*>(initStack) - m_numMaxPages * stack_allocator_interface::s_pageSize };
}
void stack_allocator::deallocate(const detail::execution_stack& i_stack) const
{
	m_stackAllocImpl->deallocate(i_stack.get_end(),i_stack.get_size() / stack_allocator_interface::s_pageSize);
	m_stackAllocImpl->release(i_stack.get_init(),m_numMaxPages);
}
size_t stack_allocator::get_num_max_pages() const
{
	return m_numMaxPages;
}
size_t stack_allocator::get_num_guard_pages() const
{
	return m_stackAllocImpl->get_num_guard_pages();
}
stack_alloc_const_dist_ref stack_allocator::get_alloc_impl() const
{
	return m_stackAllocImpl;
}
stack_alloc_const_lent_ref stack_allocator::get_alloc_impl_ref() const
{
	return lend(m_stackAllocImpl);
}

}
