#pragma once

#include "ddk_iwaiting_room.h"
#include <pthread.h>

namespace ddk
{

class reader_waiting_room : public iwaiting_room
{
public:
	reader_waiting_room(SharedState& i_sharedState);
	~reader_waiting_room();

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

}