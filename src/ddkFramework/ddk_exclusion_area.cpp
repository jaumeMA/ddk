#include "ddk_exclusion_area.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

exclusion_area::exclusion_area()
: m_waitingRoom({ddk::make_unique_reference<reader_waiting_room>(m_sharedState),ddk::make_unique_reference<writer_waiting_room>(m_sharedState) })
{
}
exclusion_area::exclusion_area(exclusion_area&& other)
: m_waitingRoom(std::move(other.m_waitingRoom))
{
}
void exclusion_area::enterReader(Reentrancy i_reentrancy)
{
	m_waitingRoom[iwaiting_room::Reader]->enter(i_reentrancy);
}
bool exclusion_area::tryToEnterReader(Reentrancy i_reentrancy)
{
	return m_waitingRoom[iwaiting_room::Reader]->tryToEnter(i_reentrancy);
}
void exclusion_area::leaverReader()
{
	m_waitingRoom[iwaiting_room::Reader]->leave();
}
void exclusion_area::enterWriter(Reentrancy i_reentrancy)
{
	m_waitingRoom[iwaiting_room::Writer]->enter(i_reentrancy);
}
bool exclusion_area::tryToEnterWriter(Reentrancy i_reentrancy)
{
	return m_waitingRoom[iwaiting_room::Writer]->tryToEnter(i_reentrancy);
}
void exclusion_area::leaveWriter()
{
	m_waitingRoom[iwaiting_room::Writer]->leave();
}
const iwaiting_room::SharedState& exclusion_area::getReaderState() const
{
	return m_waitingRoom[iwaiting_room::Reader]->getSharedState();
}
const iwaiting_room::SharedState& exclusion_area::getWriterState() const
{
	return m_waitingRoom[iwaiting_room::Writer]->getSharedState();
}

}
