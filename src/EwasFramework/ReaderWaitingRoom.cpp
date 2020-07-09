#include "ReaderWaitingRoom.h"
#include "WASLogger.h"
#include <system_error>
#include "thread_utils.h"
#include <thread>

ReaderWaitingRoom::ReaderWaitingRoom(SharedState& i_sharedState)
: m_numParticipants(0)
, m_blockedReader(false)
, m_numWaitingReaders(0)
, m_sharedState(i_sharedState)
{
	pthread_mutex_init(&m_stateRoomMutex, NULL);
	pthread_cond_init(&m_condVariable, NULL);
}
ReaderWaitingRoom::~ReaderWaitingRoom()
{
	pthread_cond_destroy(&m_condVariable);
	pthread_mutex_destroy(&m_stateRoomMutex);
}
void ReaderWaitingRoom::_enter_area(Reentrancy i_reentrancy)
{
	//WAS_LOG_DEBUG("ReaderWaitingRoom::enter() ...");
	pthread_mutex_lock(&m_stateRoomMutex);

#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.addStackTrace(Reader);
#endif

	m_numParticipants++;

	if(m_sharedState.try_acquire_lock(Reader) == EBUSY)
	{
		const bool hasWaitingWriters = i_reentrancy == Reentrancy::NON_REENTRANT && m_sharedState.hasWaitingWriters();
		const bool hasPendingReaders = m_blockedReader || m_numWaitingReaders > 0;
		const bool shallWait = (i_reentrancy == Reentrancy::NON_REENTRANT && hasPendingReaders) || m_blockedReader;

		if(m_numParticipants == 1 || (hasWaitingWriters && !hasPendingReaders))
		{
			m_blockedReader = true;

			pthread_mutex_unlock(&m_stateRoomMutex);

			m_sharedState.acquire_lock(Reader);

			while(m_sharedState.hasWaitingWriters())
			{
				m_sharedState.release_lock(Reader);

				std::this_thread::yield();

				m_sharedState.acquire_lock(Reader);
			}

			pthread_mutex_lock(&m_stateRoomMutex);

			m_blockedReader = false;

			if (m_numWaitingReaders > 0)
			{
				pthread_cond_broadcast(&m_condVariable);
			}
		}
		else if(shallWait)
		{
			++m_numWaitingReaders;

			pthread_cond_wait(&m_condVariable,&m_stateRoomMutex);

			--m_numWaitingReaders;

			//posix cannot garuantee the atomic operations between condition variable wake up, effective wake up of slept thread and any other entering thread, so:
			if (m_numWaitingReaders > 0)
			{
				pthread_cond_broadcast(&m_condVariable);
			}
		}
	}

	m_sharedState.setCurrentState(Reader);

	pthread_mutex_unlock(&m_stateRoomMutex);
	//WAS_LOG_DEBUG("ReaderWaitingRoom::enter() ... success");
}
bool ReaderWaitingRoom::_try_to_enter_area(Reentrancy i_reentrancy)
{
	//WAS_LOG_DEBUG("ReaderWaitingRoom::tryToEnter() ...");
	pthread_mutex_lock(&m_stateRoomMutex);

	bool res = false;

	if(m_sharedState.try_acquire_lock(Reader) == EBUSY)
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

	pthread_mutex_unlock(&m_stateRoomMutex);

	//WAS_LOG_DEBUG("ReaderWaitingRoom::tryToEnter() ... result = " << res);
	return res;
}
void ReaderWaitingRoom::_leave_area()
{
	//WAS_LOG_DEBUG("ReaderWaitingRoom::leave() ...");
	pthread_mutex_lock(&m_stateRoomMutex);

	EWAS_ASSERT(m_numParticipants > 0, "Trying to leave non occupied reader waiting room");

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

	pthread_mutex_unlock(&m_stateRoomMutex);
	//WAS_LOG_DEBUG("ReaderWaitingRoom::leave() ... success");
}
const IWaitingRoom::SharedState& ReaderWaitingRoom::getSharedState() const
{
	return m_sharedState;
}
