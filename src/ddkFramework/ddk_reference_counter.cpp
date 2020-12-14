#include "ddk_reference_counter.h"
#include "ddk_lock_guard.h"

#include <sstream>

#ifdef DDK_DEBUG

#if defined(WIN32)

#include <dbghelp.h>

#elif defined(__LINUX__)

#include <execinfo.h>

#endif

#endif

namespace ddk
{

#ifdef DDK_DEBUG

//#define TRACK_STACK

detail::symbol_cache_table lent_reference_counter::m_symbolInfoCache = detail::symbol_cache_table();

#endif

lent_reference_counter::lent_reference_counter()
#ifdef DDK_DEBUG
	: m_numWeakReferences(0)
#endif
{
}
lent_reference_counter::lent_reference_counter(const lent_reference_counter& other)
#ifdef DDK_DEBUG
	: m_numWeakReferences(other.m_numWeakReferences)
#endif
{
}
lent_reference_counter::lent_reference_counter(lent_reference_counter&& other)
#ifdef DDK_DEBUG
: m_numWeakReferences(0)
#endif
{
#ifdef DDK_DEBUG
	m_numWeakReferences.set(other.m_numWeakReferences.get());
	other.m_numWeakReferences.set(0);
#ifdef TRACK_STACK
	std::swap(m_stackTraces,other.m_stackTraces);
#endif
#endif
}
#ifdef DDK_DEBUG
size_t lent_reference_counter::incrementWeakReference()
{
	return atomic_post_increment(m_numWeakReferences);
}
size_t lent_reference_counter::decrementWeakReference()
{
	return atomic_post_decrement(m_numWeakReferences);
}
size_t lent_reference_counter::getNumWeakReferences() const
{
	return m_numWeakReferences.get();
}
bool lent_reference_counter::hasWeakReferences() const
{
	return m_numWeakReferences.get() > 0;
}
void lent_reference_counter::registerStackTrace(size_t i_id)
{
#ifdef TRACK_STACK
	typedef detail::symbol_cache_table::symbol_info symbol_info;

	static const size_t s_maxFuncNameLength = 64;

	void* stack[k_maxNumberOfStacks] = { NULL };

#ifdef WIN32
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	WORD numberOfFrames = CaptureStackBackTrace(3, k_maxNumberOfStacks, stack, NULL);

#elif defined (__LINUX__)
	int numberOfFrames = backtrace(stack,k_maxNumberOfStacks);
#else
#error "Pending to be implemented for this platform"
#endif

	lock_guard lg(m_refMutex);

	stack_entry& currStackTrace = m_stackTraces[i_id];

	for (int i = 0; i < k_maxNumberOfStacks; i++)
	{
		if(i<numberOfFrames)
		{
			const symbol_info& symbolicatedSymbol = m_symbolInfoCache.symbolicate(stack[i]);

			currStackTrace[i] = stack[i];
		}
		else
		{
			currStackTrace[i] = nullptr;
		}
	}

#endif
}
void lent_reference_counter::unregisterStackTrace(size_t i_id)
{
#ifdef TRACK_STACK
	lock_guard lg(m_refMutex);

	stack_container::iterator itStackTrace = m_stackTraces.find(i_id);
	if (itStackTrace != m_stackTraces.end())
	{
		m_stackTraces.erase(itStackTrace);
	}
#endif
}
void lent_reference_counter::copyStackTrace(size_t i_oldId, size_t i_newId)
{
#ifdef TRACK_STACK
	lock_guard lg(m_refMutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if (itOldStackTrace != m_stackTraces.end())
	{		
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
	}
#endif
}
void lent_reference_counter::reassignStackTrace(size_t i_oldId, size_t i_newId)
{
#ifdef TRACK_STACK
	lock_guard lg(m_refMutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if (itOldStackTrace != m_stackTraces.end())
	{
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
		m_stackTraces.erase(itOldStackTrace);
	}
#endif
}
lent_reference_counter::stack_contents lent_reference_counter::dumpStackTrace()
{
	stack_contents res;

	lock_guard lg(m_refMutex);

	stack_container::iterator itStackTrace = m_stackTraces.begin();
	for(;itStackTrace!=m_stackTraces.end();++itStackTrace)
	{
		stack_entry& currStack = itStackTrace->second;

		for (size_t i = 0; i < k_maxNumberOfStacks; i++)
		{
			if(void* currStackSymbol = currStack[i])
			{
				const detail::symbol_cache_table::symbol_info& symbolicatedSymbol = m_symbolInfoCache.symbolicate(currStackSymbol);
				//std stringstream is just too slow
				char contents[k_maxNumOfChars] = { 0 };
				sprintf(contents, "%s(%zu) : %s\n", symbolicatedSymbol.m_filename.c_str(), symbolicatedSymbol.m_lineNumber, symbolicatedSymbol.m_name.c_str());

				res[itStackTrace->first].emplace_back(contents);
			}
		}
	}

	return res;
}

#endif

}
