#pragma once

#include "ddk_iwaiting_room.h"
#include <pthread.h>

namespace ddk
{

class writer_waiting_room : public iwaiting_room
{
public:
	writer_waiting_room(SharedState& i_sharedState);
	~writer_waiting_room();

private:
	virtual void _enter_area(Reentrancy i_reentrancy) override;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) override;
	virtual void _leave_area() override;
	virtual const SharedState& getSharedState() const override;

	size_t				m_numParticipants;
	SharedState&		m_sharedState;
	pthread_mutex_t		m_stateRoomMutex;
};

}