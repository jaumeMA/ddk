#include "ddk_ucontext.h"
#include <stdarg.h>
#include <stdio.h>
#include <utility>

namespace ddk
{

#if defined(WIN32)

namespace detail
{

void unwind_calling_context(ucontext_t* i_context)
{
	ULONG64 imageBase = 0;
	UNWIND_HISTORY_TABLE unwindTable;
	RtlZeroMemory(&unwindTable,sizeof(UNWIND_HISTORY_TABLE));

	if(PRUNTIME_FUNCTION currFunc = RtlLookupFunctionEntry(i_context->uc_mcontext.Rip,&imageBase,&unwindTable))
	{
		void* hdl = nullptr;
		DWORD64 stablisher = 0;

		RtlVirtualUnwind(0,imageBase,i_context->uc_mcontext.Rip,currFunc,&(i_context->uc_mcontext),&hdl,&stablisher,nullptr);
	}
	else
	{
		i_context->uc_mcontext.Rip = *(ULONG_PTR *)i_context->uc_mcontext.Rsp;
		i_context->uc_mcontext.Rsp += 8;
	}
}
ucontext_t* get_return_context()
{
	thread_local static ucontext_t s_return_context;

	return &s_return_context;
}
void return_function()
{
	ddk::set_context(get_return_context());
}

}

#endif

int get_context(ucontext_t* i_context)
{
#if defined(WIN32)
	/* Retrieve the full machine context */
	memset(i_context,0,sizeof(ucontext_t));

    i_context->uc_mcontext.ContextFlags = CONTEXT_ALL;

	RtlCaptureContext(&(i_context->uc_mcontext));

	//at least apply one unroll for getting out from here
	detail::unwind_calling_context(i_context);

	return 0;

#else

	return getcontext(i_context);

#endif

}

int set_context(ucontext_t* i_context)
{
#if defined(WIN32)

	RtlRestoreContext(&(i_context->uc_mcontext),nullptr);

	//if you reach this point something bad happened (consult GetLastError for more info)
	return -1;

#else

	return setcontext(i_context);

#endif
}

int swap_context (ucontext_t* i_oldContext, ucontext_t* i_newContext)
{
#if defined(WIN32)

	i_oldContext->uc_mcontext.ContextFlags = CONTEXT_ALL;
	RtlCaptureContext (&i_oldContext->uc_mcontext);

	detail::unwind_calling_context(i_oldContext);

	i_oldContext->uc_mcontext.Rax = 0;

	return set_context(i_newContext);

#else

	return swapcontext(i_oldContext,i_newContext);

#endif
}

}
