#pragma once

#include "ddk_template_helper.h"

#if defined(WIN32)

#include <windows.h>

typedef struct __stack {
    void *ss_sp;
    size_t ss_size;
    int ss_flags;
} stack_t;

typedef CONTEXT mcontext_t;
typedef unsigned long __sigset_t;

typedef struct __ucontext {
    unsigned long int	uc_flags;
    struct __ucontext	*uc_link;
    stack_t				uc_stack;
    mcontext_t			uc_mcontext;
    __sigset_t			uc_sigmask;
} ucontext_t;

namespace ddk
{
namespace detail
{

inline void dump_args(ucontext_t*, uintptr_t*, size_t)
{
	//empty impl
}
template<typename Arg, typename ... Args>
void dump_args(ucontext_t* i_context, uintptr_t* i_stackPointer, size_t i_index, Arg i_arg, Args ... i_args)
{
	switch (i_index)
	{
		case 0:
			i_context->uc_mcontext.Rcx = i_arg;
			break;
		case 1:
			i_context->uc_mcontext.Rdx = i_arg;
			break;
		case 2:
			i_context->uc_mcontext.R8 = i_arg;
			break;
		case 3:
			i_context->uc_mcontext.R9 = i_arg;
			break;
		default:
			i_stackPointer[i_index] = i_arg;
			break;
	}

	dump_args(i_context,i_stackPointer,i_index+1,i_args ...);
}

}
}

#elif defined(__LINUX__)

#include <ucontext.h>

#elif defined(__APPLE__)

#define _XOPEN_SOURCE
#include <ucontext.h>

#endif

namespace ddk
{

int get_context(ucontext_t* i_context);
int set_context(ucontext_t* i_context);

template<typename ... Args>
void make_context (ucontext_t* i_context, ucontext_t* i_resumeContext, void (*i_func) (void), Args* ... i_args)
{
#if defined(WIN32)

	uintptr_t* stackPointer = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(i_context->uc_stack.ss_sp) + i_context->uc_stack.ss_size - 1);

	//align stack pointer to 16 bytes (ie 0xF)
	stackPointer = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(stackPointer) & ~0xf);
	uintptr_t* stackPointerEnd = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(i_context->uc_stack.ss_sp) & ~0xf);
	//null return address
	*stackPointer = 0;
	--stackPointer;
	//stack pointer on resume
	*stackPointer = reinterpret_cast<uintptr_t>(&(i_resumeContext->uc_mcontext));
	--stackPointer;

	i_context->uc_mcontext.Rip = reinterpret_cast<uint64_t>(i_func);
	i_context->uc_mcontext.Rsp = reinterpret_cast<uint64_t>(stackPointer);
	i_context->uc_mcontext.Rbx = reinterpret_cast<uint64_t>(stackPointerEnd);

	detail::dump_args(i_context,stackPointer,0,reinterpret_cast<uintptr_t>(i_args) ...);

#else

	i_context->uc_link = i_resumeContext;

	makecontext(i_context,i_func,mpl::get_num_types<Args...>::value,i_args...);

#endif
}

}
