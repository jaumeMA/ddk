#include "ddk_symbol_stack_walker.h"
#include "ddk_macros.h"

namespace ddk
{

symbol_stack_walker::symbol_stack_walker()
{
#if defined(WIN32)
	HANDLE process = GetCurrentProcess();
	SymInitialize(process,NULL,TRUE);
#else
    TODO("Pending");
#endif
}
#if defined(WIN32)
bool symbol_stack_walker::get_list(PCONTEXT i_context, stack_entry* i_entries, size_t i_size) const
{
	if(i_entries)
	{
		ULONG64 imageBase = 0;
		UNWIND_HISTORY_TABLE unwindTable;
		RtlZeroMemory(&unwindTable,sizeof(UNWIND_HISTORY_TABLE));

		for(size_t index = 0; index < i_size; ++index)
		{
			if(PRUNTIME_FUNCTION currFunc = RtlLookupFunctionEntry(i_context->Rip,&imageBase,&unwindTable))
			{
				void* hdl = nullptr;
				ULONG64 stablisher = 0;

				RtlVirtualUnwind(0,imageBase,i_context->Rip,currFunc,i_context,&hdl,&stablisher,nullptr);

				i_entries[index] = (void*)i_context->Rip;
			}
			else
			{
				break;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}
#endif

}
