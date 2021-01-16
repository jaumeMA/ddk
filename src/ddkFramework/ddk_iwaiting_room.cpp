#include "ddk_iwaiting_room.h"
#include <algorithm>
#include "ddk_thread_utils.h"
#include "ddk_lock_guard.h"

#ifdef WIN32

#include <Windows.h>

#endif

#if DDK_DEBUG

#define PUSH_THREAD_ID \
	{ \
		const ddk::thread_id_t __currThreadId = ddk::get_current_thread_id(); \
		const SharedState& thisSharedState = this->getSharedState(); \
		mutex_guard lg(thisSharedState.m_localDataMutex); \
		std::vector<ddk::thread_id_t>::iterator itThreadId = std::find_if(thisSharedState.m_mutexOwnerThreadId.begin(), thisSharedState.m_mutexOwnerThreadId.end(), [&__currThreadId](const ddk::thread_id_t i_threadId){ return ddk::thread_id_equal(i_threadId,__currThreadId); }); \
		DDK_ASSERT(itThreadId == thisSharedState.m_mutexOwnerThreadId.end(), "It is forbidden reentrancy of the same thread against the same exclusion area"); \
		thisSharedState.m_mutexOwnerThreadId.push_back(__currThreadId); \
	}
#define POP_THREAD_ID \
	{ \
		const ddk::thread_id_t __currThreadId = ddk::get_current_thread_id(); \
		const SharedState& thisSharedState = this->getSharedState(); \
		mutex_guard lg(thisSharedState.m_localDataMutex); \
		std::vector<ddk::thread_id_t>::iterator itThreadId = std::find_if(thisSharedState.m_mutexOwnerThreadId.begin(), thisSharedState.m_mutexOwnerThreadId.end(), [&__currThreadId](const ddk::thread_id_t i_threadId){ return ddk::thread_id_equal(i_threadId,__currThreadId); }); \
		DDK_ASSERT(itThreadId != thisSharedState.m_mutexOwnerThreadId.end(), "Unexistent thread id!"); \
		if (itThreadId != thisSharedState.m_mutexOwnerThreadId.end()) \
		{ \
			thisSharedState.m_mutexOwnerThreadId.erase(itThreadId); \
		} \
	}

#else

#define PUSH_THREAD_ID
#define POP_THREAD_ID

#endif


#ifdef THREAD_ACQUIRE_STACK_TRACE

#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024

#include <process.h>
#include <iostream>
#include <dbghelp.h>

#endif

namespace ddk
{

iwaiting_room::SharedState::SharedState()
: m_currentState(None)
, m_numWaitingWriters(0)
{
}
void iwaiting_room::enter(Reentrancy i_reentrancy)
{
	PUSH_THREAD_ID

	_enter_area(i_reentrancy);
}
bool iwaiting_room::tryToEnter(Reentrancy i_reentrancy)
{
	if (_try_to_enter_area(i_reentrancy))
	{
		PUSH_THREAD_ID

		return true;
	}

	return false;
}
void iwaiting_room::leave()
{
	POP_THREAD_ID

	_leave_area();
}
bool iwaiting_room::SharedState::hasWaitingWriters() const
{
	return m_numWaitingWriters.get() > 0;
}
size_t iwaiting_room::SharedState::getNumWaitingWriters() const
{
	return m_numWaitingWriters.get();
}
void iwaiting_room::SharedState::acquire_lock(Access i_access)
{
	if(i_access == Writer)
	{
		ddk::atomic_post_increment(m_numWaitingWriters);
	}

	m_exclusiveMutex.lock();

	if(i_access == Writer)
	{
		ddk::atomic_post_decrement(m_numWaitingWriters);
	}
}
void iwaiting_room::SharedState::release_lock(Access i_access)
{
	m_exclusiveMutex.unlock();
}
int iwaiting_room::SharedState::try_acquire_lock(Access i_access)
{
	return m_exclusiveMutex.try_lock();
}
iwaiting_room::SharedState::Access iwaiting_room::SharedState::getCurrentState() const
{
	return m_currentState;
}
void iwaiting_room::SharedState::setCurrentState(Access i_state)
{
	m_currentState = i_state;
}
int iwaiting_room::SharedState::GetThreadId() const
{
	return CURR_THREAD_ID;
}

#ifdef THREAD_ACQUIRE_STACK_TRACE

void iwaiting_room::SharedState::addStackTrace(Access i_access)
{
	const int currThread = GetCurrentThreadId();

	std::pair<Access,std::vector<std::string>>& currStackTrace = m_stackTraceMap[currThread];

	currStackTrace.first = i_access;

	void* stack[TRACE_MAX_STACK_FRAMES];
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    WORD numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);
    SYMBOL_INFO *symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO)+(TRACE_MAX_FUNCTION_NAME_LENGTH - 1) * sizeof(TCHAR));
    symbol->MaxNameLen = TRACE_MAX_FUNCTION_NAME_LENGTH;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    DWORD displacement;
    IMAGEHLP_LINE64 *line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    for (int i = 0; i < numberOfFrames; i++)
    {
        DWORD64 address = (DWORD64)(stack[i]);
        SymFromAddr(process, address, NULL, symbol);
        if (SymGetLineFromAddr64(process, address, &displacement, line))
        {
			char contents[512] = { 0 };

			sprintf(contents, "at %s in %s: line: %lu: address: 0x%0X\n", symbol->Name, line->FileName, line->LineNumber, symbol->Address);

			currStackTrace.second.push_back(contents);
        }
    }
	free(line);
	free(symbol);
}
void iwaiting_room::SharedState::removeStackTrace()
{
	const int currentThread = GetCurrentThreadId();

	std::map<int,std::pair<Access,std::vector<std::string>>>::iterator itStackTrace = m_stackTraceMap.find(currentThread);

	if(itStackTrace != m_stackTraceMap.end())
	{
		m_stackTraceMap.erase(itStackTrace);
	}
}
size_t iwaiting_room::SharedState::numStackTraces(Access i_access) const
{
	size_t res = 0;
	const int currentThread = GetCurrentThreadId();

	std::map<int,std::pair<Access,std::vector<std::string>>>::const_iterator itStackTrace = m_stackTraceMap.begin();
	for(;itStackTrace!=m_stackTraceMap.end();++itStackTrace)
	{
		if(itStackTrace->second.first == i_access && itStackTrace->second.second.empty() == false)
		{
			res++;
		}
	}

	return res;
}
bool iwaiting_room::SharedState::hasStackTraces(Access i_access) const
{
	const int currentThread = GetCurrentThreadId();

	std::map<int,std::pair<Access,std::vector<std::string>>>::const_iterator itStackTrace = m_stackTraceMap.begin();
	for(;itStackTrace!=m_stackTraceMap.end();++itStackTrace)
	{
		if(itStackTrace->second.first == i_access && itStackTrace->second.second.empty() == false)
		{
			return true;
		}
	}

	return false;
}

#endif

}
