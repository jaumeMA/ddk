#include "ddk_reader_waiting_room.h"
#include "DDKLogger.h"
#include <system_error>
#include "ddk_thread_utils.h"
#include <thread>
#include "ddk_lock_guard.h"

namespace ddk
{

reader_waiting_room::reader_waiting_room(iwaiting_room::SharedState& i_sharedState)
: m_numParticipants(0)
, m_blockedReader(false)
, m_numWaitingReaders(0)
, m_sharedState(i_sharedState)
{
}
void reader_waiting_room::_enter_area(Reentrancy i_reentrancy)
{
	m_stateRoomMutex.lock();

#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.addStackTrace(Reader);
#endif

	m_numParticipants++;

	if(m_sharedState.try_acquire_lock(Reader) == false)
	{
		const bool hasWaitingWriters = i_reentrancy == Reentrancy::NON_REENTRANT && m_sharedState.hasWaitingWriters();
		const bool hasPendingReaders = m_blockedReader || m_numWaitingReaders > 0;
		const bool shallWait = (i_reentrancy == Reentrancy::NON_REENTRANT && hasPendingReaders) || m_blockedReader;

		if(m_numParticipants == 1 || (hasWaitingWriters && !hasPendingReaders))
		{
			m_blockedReader = true;

			m_stateRoomMutex.unlock();

			m_sharedState.acquire_lock(Reader);

			while(m_sharedState.hasWaitingWriters())
			{
				m_sharedState.release_lock(Reader);

				std::this_thread::yield();

				m_sharedState.acquire_lock(Reader);
			}

			m_stateRoomMutex.lock();

			m_blockedReader = false;

			if (m_numWaitingReaders > 0)
			{
				m_condVariable.notify_all();
			}
		}
		else if(shallWait)
		{
			++m_numWaitingReaders;

			m_condVariable.wait(m_stateRoomMutex);

			--m_numWaitingReaders;

			//posix cannot garuantee the atomic operations between condition variable wake up, effective wake up of slept thread and any other entering thread, so:
			if (m_numWaitingReaders > 0)
			{
				m_condVariable.notify_all();
			}
		}
	}

	m_sharedState.setCurrentState(Reader);

	m_stateRoomMutex.unlock();
}
bool reader_waiting_room::_try_to_enter_area(Reentrancy i_reentrancy)
{
	mutex_guard lg(m_stateRoomMutex);

	bool res = false;

	if(m_sharedState.try_acquire_lock(Reader) == false)
	{
		res = (m_blockedReader == false) && (i_reentrancy == Reentrancy::REENTRANT || m_sharedState.hasWaitingWriters() == false);
	}
	else
	{
		res = true;
	}

	if (res)
	{
		m_sharedState.setCurrentState(Reader);
		m_numParticipants++;

#ifdef THREAD_ACQUIRE_STACK_TRACE
		m_sharedState.addStackTrace(Reader);
#endif
	}

	return res;
}
void reader_waiting_room::_leave_area()
{
	mutex_guard lg(m_stateRoomMutex);

	DDK_ASSERT(m_numParticipants > 0, "Trying to leave non occupied reader waiting room");

	m_numParticipants--;

	if(m_numParticipants == 0)
	{
		m_sharedState.setCurrentState(None);
		m_sharedState.release_lock(Reader);
	}
	else if(m_blockedReader && (m_numWaitingReaders == m_numParticipants - 1))
	{
		m_sharedState.release_lock(Reader);
	}

#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.removeStackTrace();
#endif

}
const iwaiting_room::SharedState& reader_waiting_room::getSharedState() const
{
	return m_sharedState;
}

}
