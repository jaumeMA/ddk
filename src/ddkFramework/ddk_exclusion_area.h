//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_reader_waiting_room.h"
#include "ddk_writer_waiting_room.h"
#include "ddk_critical_section.h"
#include <array>

namespace ddk
{

class exclusion_area
{
public:
	exclusion_area();
	exclusion_area(const exclusion_area&) = delete;
	exclusion_area(exclusion_area&& other);
	exclusion_area& operator=(const exclusion_area&) = delete;
	exclusion_area& operator=(exclusion_area&&) = delete;

	void enterReader(Reentrancy i_reentrancy);
	bool tryToEnterReader(Reentrancy i_reentrancy);
	void leaverReader();
	void enterWriter(Reentrancy i_reentrancy);
	bool tryToEnterWriter(Reentrancy i_reentrancy);
	void leaveWriter();
	const iwaiting_room::SharedState& getReaderState() const;
	const iwaiting_room::SharedState& getWriterState() const;

private:
	iwaiting_room::SharedState m_sharedState;
	std::array<iwaiting_room_unique_ref,2> m_waitingRoom;
};

}
