#include "ddk_ucontext.h"
#include <stdarg.h>
#include <stdio.h>
#include <utility>

namespace ddk
{

#if defined(WIN32)

namespace detail
{

void unwind_calling_context(mcontext_t* i_context)
{
	ULONG64 imageBase = 0;
	UNWIND_HISTORY_TABLE unwindTable;
	RtlZeroMemory(&unwindTable,sizeof(UNWIND_HISTORY_TABLE));

	if(PRUNTIME_FUNCTION currFunc = RtlLookupFunctionEntry(i_context->Rip,&imageBase,&unwindTable))
	{
		void* hdl = nullptr;
		ULONG64 stablisher = 0;

		RtlVirtualUnwind(0,imageBase,i_context->Rip,currFunc,i_context,&hdl,&stablisher,nullptr);
	}
	else
	{
		i_context->Rip = *(ULONG_PTR *)(i_context->Rsp);
		i_context->Rsp += 8;
	}
}

}

#endif

int get_context(ucontext_t* i_context)
{
#if defined(WIN32)

	RtlZeroMemory(&(i_context->uc_mcontext),sizeof(mcontext_t));

    i_context->uc_mcontext.ContextFlags = CONTEXT_CONTROL;

	RtlCaptureContext(&(i_context->uc_mcontext));

	//at least apply one unroll for getting out from here
	detail::unwind_calling_context(&(i_context->uc_mcontext));

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

}
