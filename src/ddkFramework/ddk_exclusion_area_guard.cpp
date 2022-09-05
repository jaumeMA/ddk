#include "ddk_exclusion_area_guard.h"

namespace ddk
{

guard_reader::guard_reader(exclusion_area& i_exclArea)
: m_exclArea(i_exclArea)
{
	m_exclArea.enterReader(Reentrancy::NON_REENTRANT);
}
guard_reader::~guard_reader()
{
	m_exclArea.leaverReader();
}

guard_writer::guard_writer(exclusion_area& i_exclArea)
: m_exclArea(i_exclArea)
{
	m_exclArea.enterWriter(Reentrancy::NON_REENTRANT);
}
guard_writer::~guard_writer()
{
	m_exclArea.leaveWriter();
}

}