#include "ddk_symbol_cache_table.h"
#include "ddk_lock_guard.h"

#ifdef WIN32

#include <windows.h>
#include <dbghelp.h>

#endif

namespace ddk
{
namespace detail
{

symbol_cache_table::symbol_info::operator bool() const
{
	return (m_filename.empty() == false) && (m_name.empty() == false);
}

symbol_cache_table::const_reference symbol_cache_table::symbolicate(void* i_address)
{
	const size_t symbolKey = reinterpret_cast<size_t>(i_address);

	mutex_guard lg(m_refMutex);

	iterator itSymbol = m_symbolInfo.find(symbolKey);

	if (itSymbol == m_symbolInfo.end())
	{
		symbol_info symbolicatedSymbol;

#ifdef WIN32
		static const size_t s_maxFuncNameLength = 64;
		static char symbolArena[sizeof(SYMBOL_INFO) + (s_maxFuncNameLength - 1)*sizeof(TCHAR)] = {0};
		static IMAGEHLP_LINE64 line = {0};

		HANDLE process = GetCurrentProcess();
		SYMBOL_INFO *symbol = reinterpret_cast<SYMBOL_INFO *>(symbolArena);
		DWORD displacement;

		symbol->MaxNameLen = s_maxFuncNameLength;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		DWORD64 address = (DWORD64)(i_address);
		SymFromAddr(process, address, NULL, symbol);


		if (SymGetLineFromAddr64(process, address, &displacement, &line))
		{
			symbolicatedSymbol.m_filename = line.FileName;
			symbolicatedSymbol.m_name = symbol->Name;
			symbolicatedSymbol.m_lineNumber = line.LineNumber;
		}
#else
	#warning "Pending to be implemented for this platform"
#endif

		std::pair<iterator,bool> res = m_symbolInfo.insert(std::make_pair(symbolKey,symbolicatedSymbol));

		return res.first->second;
	}
	else
	{
		return itSymbol->second;
	}
}

}
}
