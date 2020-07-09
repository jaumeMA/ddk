#include "IWaitingRoom.h"
#include <algorithm>
#include "thread_utils.h"

#ifdef WIN32

#include <Windows.h>

#endif

#if EWAS_DEBUG

#define PUSH_THREAD_ID \
	{ \
		const ewas::thread_id_t __currThreadId = ewas::get_current_thread_id(); \
		const SharedState& thisSharedState = this->getSharedState(); \
		pthread_mutex_lock(&thisSharedState.m_localDataMutex); \
		std::vector<ewas::thread_id_t>::iterator itThreadId = std::find_if(thisSharedState.m_mutexOwnerThreadId.begin(), thisSharedState.m_mutexOwnerThreadId.end(), [&__currThreadId](const ewas::thread_id_t i_threadId){ return ewas::thread_id_equal(i_threadId,__currThreadId); }); \
		EWAS_ASSERT(itThreadId == thisSharedState.m_mutexOwnerThreadId.end(), "It is forbidden reentrancy of the same thread against the same exclusion area"); \
		thisSharedState.m_mutexOwnerThreadId.push_back(__currThreadId); \
		pthread_mutex_unlock(&thisSharedState.m_localDataMutex); \
	}
#define POP_THREAD_ID \
	{ \
		const ewas::thread_id_t __currThreadId = ewas::get_current_thread_id(); \
		const SharedState& thisSharedState = this->getSharedState(); \
		pthread_mutex_lock(&thisSharedState.m_localDataMutex); \
		std::vector<ewas::thread_id_t>::iterator itThreadId = std::find_if(thisSharedState.m_mutexOwnerThreadId.begin(), thisSharedState.m_mutexOwnerThreadId.end(), [&__currThreadId](const ewas::thread_id_t i_threadId){ return ewas::thread_id_equal(i_threadId,__currThreadId); }); \
		EWAS_ASSERT(itThreadId != thisSharedState.m_mutexOwnerThreadId.end(), "Unexistent thread id!"); \
		if (itThreadId != thisSharedState.m_mutexOwnerThreadId.end()) \
		{ \
			thisSharedState.m_mutexOwnerThreadId.erase(itThreadId); \
		} \
		pthread_mutex_unlock(&thisSharedState.m_localDataMutex); \
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

IWaitingRoom::SharedState::SharedState()
: m_currentState(None)
, m_numWaitingWriters(0)
{
	pthread_mutex_init(&m_exclusiveMutex,NULL);

#ifdef EWAS_DEBUG
	pthread_mutex_init(&m_localDataMutex,NULL);
#endif
}
IWaitingRoom::SharedState::~SharedState()
{
#ifdef EWAS_DEBUG
	pthread_mutex_destroy(&m_localDataMutex);
#endif

	pthread_mutex_destroy(&m_exclusiveMutex);
}
void IWaitingRoom::enter(Reentrancy i_reentrancy)
{
	PUSH_THREAD_ID

	_enter_area(i_reentrancy);
}
bool IWaitingRoom::tryToEnter(Reentrancy i_reentrancy)
{
	if (_try_to_enter_area(i_reentrancy))
	{
		PUSH_THREAD_ID

		return true;
	}

	return false;
}
void IWaitingRoom::leave()
{
	POP_THREAD_ID

	_leave_area();
}
bool IWaitingRoom::SharedState::hasWaitingWriters() const
{
	return m_numWaitingWriters.get() > 0;
}
size_t IWaitingRoom::SharedState::getNumWaitingWriters() const
{
	return m_numWaitingWriters.get();
}
void IWaitingRoom::SharedState::acquire_lock(Access i_access)
{
	if(i_access == Writer)
	{
		ewas::atomic_post_increment(m_numWaitingWriters);
	}

	pthread_mutex_lock(&m_exclusiveMutex);

	if(i_access == Writer)
	{
		ewas::atomic_post_decrement(m_numWaitingWriters);
	}
}
void IWaitingRoom::SharedState::release_lock(Access i_access)
{
	pthread_mutex_unlock(&m_exclusiveMutex);
}
int IWaitingRoom::SharedState::try_acquire_lock(Access i_access)
{
	return pthread_mutex_trylock(&m_exclusiveMutex);
}
IWaitingRoom::SharedState::Access IWaitingRoom::SharedState::getCurrentState() const
{
	return m_currentState;
}
void IWaitingRoom::SharedState::setCurrentState(Access i_state)
{
	m_currentState = i_state;
}
int IWaitingRoom::SharedState::GetThreadId() const
{
	return CURR_THREAD_ID;
}

#ifdef THREAD_ACQUIRE_STACK_TRACE

void IWaitingRoom::SharedState::addStackTrace(Access i_access)
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
void IWaitingRoom::SharedState::removeStackTrace()
{
	const int currentThread = GetCurrentThreadId();

	std::map<int,std::pair<Access,std::vector<std::string>>>::iterator itStackTrace = m_stackTraceMap.find(currentThread);

	if(itStackTrace != m_stackTraceMap.end())
	{
		m_stackTraceMap.erase(itStackTrace);
	}
}
size_t IWaitingRoom::SharedState::numStackTraces(Access i_access) const
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
bool IWaitingRoom::SharedState::hasStackTraces(Access i_access) const
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
