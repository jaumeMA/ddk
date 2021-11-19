#pragma once

#if defined(WIN32)

#pragma comment(lib, "dbghelp")
#include <windows.h>
#include <dbghelp.h>

#endif // defined

namespace ddk
{

class symbol_stack_walker
{
public:
	typedef void* stack_entry;

	symbol_stack_walker();

#if defined(WIN32)
	bool get_list(PCONTEXT i_context, stack_entry* i_entries, size_t i_size) const;
#endif
};

}

#include "ddk_symbol_stack_walker.inl"