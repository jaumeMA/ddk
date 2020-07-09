#include "ExclusionArea.h"

ExclusionArea::ExclusionArea()
{
	//create waiting rooms
	m_waitingRoom[IWaitingRoom::Reader] = new ReaderWaitingRoom(m_sharedState);
	m_waitingRoom[IWaitingRoom::Writer] = new WriterWaitingRoom(m_sharedState);
}
ExclusionArea::~ExclusionArea()
{
	delete m_waitingRoom[IWaitingRoom::Reader];
	delete m_waitingRoom[IWaitingRoom::Writer];
}
void ExclusionArea::enterReader(Reentrancy i_reentrancy)
{
	m_waitingRoom[IWaitingRoom::Reader]->enter(i_reentrancy);
}
bool ExclusionArea::tryToEnterReader(Reentrancy i_reentrancy)
{
	return m_waitingRoom[IWaitingRoom::Reader]->tryToEnter(i_reentrancy);
}
void ExclusionArea::leaverReader()
{
	m_waitingRoom[IWaitingRoom::Reader]->leave();
}
void ExclusionArea::enterWriter(Reentrancy i_reentrancy)
{
	m_waitingRoom[IWaitingRoom::Writer]->enter(i_reentrancy);
}
bool ExclusionArea::tryToEnterWriter(Reentrancy i_reentrancy)
{
	return m_waitingRoom[IWaitingRoom::Writer]->tryToEnter(i_reentrancy);
}
void ExclusionArea::leaveWriter()
{
	m_waitingRoom[IWaitingRoom::Writer]->leave();
}
const IWaitingRoom::SharedState& ExclusionArea::getReaderState() const
{
	return m_waitingRoom[IWaitingRoom::Reader]->getSharedState();
}
const IWaitingRoom::SharedState& ExclusionArea::getWriterState() const
{
	return m_waitingRoom[IWaitingRoom::Writer]->getSharedState();
}