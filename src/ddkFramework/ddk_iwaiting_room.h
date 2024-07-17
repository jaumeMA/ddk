//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_macros.h"
#include "ddk_atomics.h"
#include "ddk_critical_section.h"
#include "ddk_mutex.h"
#include "ddk_unique_reference_wrapper.h"
#include <vector>

#undef THREAD_ACQUIRE_STACK_TRACE

#ifdef THREAD_ACQUIRE_STACK_TRACE

#include <map>
#include <string>

#endif

namespace ddk
{

class iwaiting_room
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
		friend class iwaiting_room;
	public:
		typedef iwaiting_room::Access Access;

		SharedState();

		void acquire_lock(Access i_access);
		void release_lock(Access i_access);
		bool try_acquire_lock(Access i_access);
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

#ifdef DDK_DEBUG
		mutable std::vector<ddk::thread_id_t> m_mutexOwnerThreadId;
		mutable mutex m_localDataMutex;
#endif

	private:
		int GetThreadId() const;

		mutex m_exclusiveMutex;
		Access m_currentState;
		ddk::atomic_size_t m_numWaitingWriters;
#ifdef THREAD_ACQUIRE_STACK_TRACE
		std::map<int,std::pair<Access,std::vector<std::string>>> m_stackTraceMap;
#endif
	};

	virtual ~iwaiting_room(){};
	void enter(Reentrancy i_reentrancy);
	bool tryToEnter(Reentrancy i_reentrancy);
	void leave();
	virtual const SharedState& getSharedState() const = 0;

private:
	virtual void _enter_area(Reentrancy i_reentrancy) = 0;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) = 0;
	virtual void _leave_area() = 0;
};

typedef ddk::unique_reference_wrapper<iwaiting_room> iwaiting_room_unique_ref;
typedef ddk::unique_reference_wrapper<const iwaiting_room> iwaiting_room_const_unique_ref;

}