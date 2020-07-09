#pragma once

#include "IWaitingRoom.h"
#include <pthread.h>

class WriterWaitingRoom : public IWaitingRoom
{
public:
	WriterWaitingRoom(SharedState& i_sharedState);
	~WriterWaitingRoom();

private:
	virtual void _enter_area(Reentrancy i_reentrancy) override;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) override;
	virtual void _leave_area() override;
	virtual const SharedState& getSharedState() const override;

	size_t				m_numParticipants;
	SharedState&		m_sharedState;
	pthread_mutex_t		m_stateRoomMutex;
};
