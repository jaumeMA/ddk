#pragma once

#include "ReaderWaitingRoom.h"
#include "WriterWaitingRoom.h"
#include "CriticalSection.h"
#include <array>

class ExclusionArea
{
public:
	ExclusionArea();
	~ExclusionArea();

	void enterReader(Reentrancy i_reentrancy);
	bool tryToEnterReader(Reentrancy i_reentrancy);
	void leaverReader();
	void enterWriter(Reentrancy i_reentrancy);
	bool tryToEnterWriter(Reentrancy i_reentrancy);
	void leaveWriter();
	const IWaitingRoom::SharedState& getReaderState() const;
	const IWaitingRoom::SharedState& getWriterState() const;

private:
	IWaitingRoom::SharedState m_sharedState;
	std::array<IWaitingRoom*,2> m_waitingRoom;
};
