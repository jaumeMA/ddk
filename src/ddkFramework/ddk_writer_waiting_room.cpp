#include "ddk_writer_waiting_room.h"
#include "DDKLogger.h"

namespace ddk
{

writer_waiting_room::writer_waiting_room(SharedState& i_sharedState)
: m_numParticipants(0)
, m_sharedState(i_sharedState)
{
	pthread_mutex_init(&m_stateRoomMutex, NULL);
}
writer_waiting_room::~writer_waiting_room()
{
	pthread_mutex_destroy(&m_stateRoomMutex);
}
void writer_waiting_room::_enter_area(Reentrancy i_reentrancy)
{
#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.addStackTrace(Writer);
#endif

	pthread_mutex_lock(&m_stateRoomMutex);

	m_numParticipants++;

	pthread_mutex_unlock(&m_stateRoomMutex);

	m_sharedState.acquire_lock(Writer);

	m_sharedState.setCurrentState(Writer);
}
bool writer_waiting_room::_try_to_enter_area(Reentrancy i_reentrancy)
{
	bool res = false;

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

	pthread_mutex_unlock(&m_stateRoomMutex);

	return res;
}
void writer_waiting_room::_leave_area()
{
#ifdef THREAD_ACQUIRE_STACK_TRACE
	m_sharedState.removeStackTrace();
#endif

	pthread_mutex_lock(&m_stateRoomMutex);

	DDK_ASSERT(m_numParticipants > 0, "Trying to leave non occupied writer waiting room");

	m_numParticipants--;

	pthread_mutex_unlock(&m_stateRoomMutex);

	m_sharedState.setCurrentState(None);

	m_sharedState.release_lock(Writer);
}
const iwaiting_room::SharedState& writer_waiting_room::getSharedState() const
{
	return m_sharedState;
}

}