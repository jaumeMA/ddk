#include "WriterWaitingRoom.h"
#include "WASLogger.h"

WriterWaitingRoom::WriterWaitingRoom(SharedState& i_sharedState)
: m_numParticipants(0)
, m_sharedState(i_sharedState)
{
	pthread_mutex_init(&m_stateRoomMutex, NULL);
}
WriterWaitingRoom::~WriterWaitingRoom()
{
	pthread_mutex_destroy(&m_stateRoomMutex);
}
void WriterWaitingRoom::_enter_area(Reentrancy i_reentrancy)
{
#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.addStackTrace(Writer);
#endif

	//WAS_LOG_DEBUG("WriterWaitingRoom::enter() ...");
	pthread_mutex_lock(&m_stateRoomMutex);
	m_numParticipants++;
	pthread_mutex_unlock(&m_stateRoomMutex);

	m_sharedState.acquire_lock(Writer);

	m_sharedState.setCurrentState(Writer);
	//WAS_LOG_DEBUG("WriterWaitingRoom::enter() ... success");
}
bool WriterWaitingRoom::_try_to_enter_area(Reentrancy i_reentrancy)
{
	bool res = false;

	//WAS_LOG_DEBUG("WriterWaitingRoom::tryToEnter() ...");
	pthread_mutex_lock(&m_stateRoomMutex);

	if(m_sharedState.try_acquire_lock(Writer) == 0)
	{
		m_numParticipants++;

		m_sharedState.setCurrentState(Writer);

#ifdef THREAD_ACQUIRE_STACK_TRACE
		m_sharedState.addStackTrace(Writer);
#endif
		res = true;
	}
	//WAS_LOG_WARN("WriterWaitingRoom::enter() - try_acquire_lock(" << &m_sharedState << ") end");

	pthread_mutex_unlock(&m_stateRoomMutex);

	//WAS_LOG_DEBUG("WriterWaitingRoom::tryToEnter() ... result " << res);
	return res;
}
void WriterWaitingRoom::_leave_area()
{
#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.removeStackTrace();
#endif
	//WAS_LOG_DEBUG("WriterWaitingRoom::leave() ...");
	pthread_mutex_lock(&m_stateRoomMutex);
	EWAS_ASSERT(m_numParticipants > 0, "Trying to leave non occupied writer waiting room");
	m_numParticipants--;
	TODO("JAUME: investigate why these lines fail from time to time while matches condition")
	//EWAS_ASSERT(m_numParticipants == m_sharedState.getNumWaitingWriters(), "Trying to leave non occupied writer waiting room");
	pthread_mutex_unlock(&m_stateRoomMutex);

	m_sharedState.setCurrentState(None);

	m_sharedState.release_lock(Writer);
	//WAS_LOG_DEBUG("WriterWaitingRoom::leave() ... success");
}
const IWaitingRoom::SharedState& WriterWaitingRoom::getSharedState() const
{
	return m_sharedState;
}