#pragma once

#include "ddk_macros.h"
#include "ddk_atomics.h"
#include "CriticalSection.h"
#include <vector>
#include <pthread.h>

#undef THREAD_ACQUIRE_STACK_TRACE

#ifdef THREAD_ACQUIRE_STACK_TRACE

#include <map>
#include <string>

#endif

class IWaitingRoom
{
public:
	enum Access
	{
		Reader,
		Writer,
		None
	};

	struct SharedState
	{
		friend class IWaitingRoom;
	public:
		typedef IWaitingRoom::Access Access;

		SharedState();
		~SharedState();

		void acquire_lock(Access i_access);
		void release_lock(Access i_access);
		int try_acquire_lock(Access i_access);
		Access getCurrentState() const;
		void setCurrentState(Access i_state);
		bool hasWaitingWriters() const;
		size_t getNumWaitingWriters() const;

#ifdef THREAD_ACQUIRE_STACK_TRACE
		void addStackTrace(Access i_access);
		void removeStackTrace();
		size_t numStackTraces(Access i_access) const;
		bool hasStackTraces(Access i_access) const;
#endif

#ifdef EWAS_DEBUG
		mutable std::vector<ewas::thread_id_t> m_mutexOwnerThreadId;
		mutable pthread_mutex_t m_localDataMutex;
#endif

	private:
		int GetThreadId() const;

		pthread_mutex_t m_exclusiveMutex;
		Access m_currentState;
		ewas::atomic_size_t m_numWaitingWriters;
#ifdef THREAD_ACQUIRE_STACK_TRACE
		std::map<int,std::pair<Access,std::vector<std::string>>> m_stackTraceMap;
#endif
	};

	virtual ~IWaitingRoom(){};
	void enter(Reentrancy i_reentrancy);
	bool tryToEnter(Reentrancy i_reentrancy);
	void leave();
	virtual const SharedState& getSharedState() const = 0;

private:
	virtual void _enter_area(Reentrancy i_reentrancy) = 0;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) = 0;
	virtual void _leave_area() = 0;
};
