#include "ddk_exclusion_area_lock.h"

namespace ddk
{

lock_reader::lock_reader(exclusion_area& i_exclArea)
: m_exclArea(i_exclArea)
{
}
void lock_reader::lock(const Reentrancy& i_reentrancy)
{
	m_exclArea.enterReader(i_reentrancy);
}
bool lock_reader::try_lock(const Reentrancy& i_reentrancy)
{
	return m_exclArea.tryToEnterReader(i_reentrancy);
}
void lock_reader::unlock()
{
	m_exclArea.leaverReader();
}

lock_writer::lock_writer(exclusion_area& i_exclArea)
: m_exclArea(i_exclArea)
{
}
void lock_writer::lock(const Reentrancy& i_reentrancy)
{
	m_exclArea.enterWriter(i_reentrancy);
}
bool lock_writer::try_lock(const Reentrancy& i_reentrancy)
{
	return m_exclArea.tryToEnterWriter(i_reentrancy);
}
void lock_writer::unlock()
{
	m_exclArea.leaveWriter();
}

}