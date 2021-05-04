#include "ddk_memory_tracker.h"
#include "ddk_lock_guard.h"
#include "ddk_macros.h"
#include "ddk_symbol_cache_table.h"

#if defined(WIN32)

#include <windows.h>
#include <dbghelp.h>

#elif defined(__LINUX__)

#include <execinfo.h>

#endif


namespace ddk
{

memory_tracker::~memory_tracker()
{
	DDK_ASSERT(m_stackTraces.empty(),"Still pending allocations while destroying system allocator");
}
memory_tracker& memory_tracker::operator=(const memory_tracker& other)
{
	mutex_guard mg(m_mutex);

	for(const auto& entry : other.m_stackTraces)
	{
		memcpy(m_stackTraces[entry.first],entry.second,sizeof(stack_entry));
	}

	return *this;
}
memory_tracker& memory_tracker::operator=(memory_tracker && other)
{
	mutex_guard mg(m_mutex);

	for(const auto& entry : other.m_stackTraces)
	{
		memcpy(m_stackTraces[entry.first],entry.second,sizeof(stack_entry));
	}
	other.m_stackTraces.clear();

	return *this;
}
void memory_tracker::register_allocation(size_t i_id)
{
	typedef detail::symbol_cache_table::symbol_info symbol_info;

	void* stack[k_maxNumberOfStacks] = { NULL };

	#ifdef WIN32
	HANDLE process = GetCurrentProcess();
	SymInitialize(process,NULL,TRUE);
	WORD numberOfFrames = CaptureStackBackTrace(3,k_maxNumberOfStacks,stack,NULL);

	#elif defined (__LINUX__)
	int numberOfFrames = backtrace(stack,k_maxNumberOfStacks);
	#else
	#error "Pending to be implemented for this platform"
	#endif

	mutex_guard mg(m_mutex);

	stack_entry& currStackTrace = m_stackTraces[i_id];

	for(int i = 0; i < k_maxNumberOfStacks; i++)
	{
		currStackTrace[i] = (i < numberOfFrames) ? stack[i] : nullptr;
	}
}
void memory_tracker::unregister_allocation(size_t i_id)
{
	mutex_guard mg(m_mutex);

	stack_container::iterator itStackTrace = m_stackTraces.find(i_id);
	if(itStackTrace != m_stackTraces.end())
	{
		m_stackTraces.erase(itStackTrace);
	}
}
void memory_tracker::copy_entries_from(size_t i_oldId,size_t i_newId)
{
	mutex_guard lg(m_mutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if(itOldStackTrace != m_stackTraces.end())
	{
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
	}
}
void memory_tracker::move_entries_to(size_t i_oldId,size_t i_newId)
{
	mutex_guard lg(m_mutex);

	stack_container::iterator itOldStackTrace = m_stackTraces.find(i_oldId);
	if(itOldStackTrace != m_stackTraces.end())
	{
		memcpy(m_stackTraces[i_newId],itOldStackTrace->second,sizeof(void*) * k_maxNumberOfStacks);
		m_stackTraces.erase(itOldStackTrace);
	}
}
void memory_tracker::clear()
{
	mutex_guard mg(m_mutex);

	m_stackTraces.clear();
}

}