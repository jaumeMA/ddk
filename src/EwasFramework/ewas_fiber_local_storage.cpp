#include "ewas_fiber_local_storage.h"
#include "ewas_macros.h"

namespace ewas
{

bool fiber_local_storage::empty(const fiber_id& i_id) const
{
	std::unordered_map<fiber_id,thread_local_storage>::const_iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		return itFiber->second.empty();
	}
	else
	{
		return true;
	}
}

}