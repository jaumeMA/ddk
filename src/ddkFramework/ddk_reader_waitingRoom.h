#pragma once

#include "IWaitingRoom.h"
#include <pthread.h>

class ReaderWaitingRoom : public IWaitingRoom
{
public:
	ReaderWaitingRoom(SharedState& i_sharedState);
	~ReaderWaitingRoom();

private:
	virtual void _enter_area(Reentrancy i_reentrancy) override;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) override;
	virtual void _leave_area() override;
	virtual const SharedState& getSharedState() const override;

	size_t				m_numParticipants;
	bool				m_blockedReader;
	size_t				m_numWaitingReaders;
	SharedState&		m_sharedState;
	pthread_mutex_t		m_stateRoomMutex;
	pthread_cond_t		m_condVariable;
};
