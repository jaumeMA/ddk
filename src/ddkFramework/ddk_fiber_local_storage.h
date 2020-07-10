#pragma once

#include <unordered_map>
#include "ewas_fiber_defs.h"
#include "ewas_thread_local_storage.h"

namespace ewas
{

class fiber_local_storage
{
public:
	fiber_local_storage() = default;

	template<typename T, typename ... Args>
	T* construct(const fiber_id& i_id, Args&& ... i_args);
	template<typename T>
	void clear(const fiber_id& i_id);
	template<typename T>
	T& get(const fiber_id& i_id);
	bool empty(const fiber_id& i_id) const;

private:
	std::unordered_map<fiber_id,thread_local_storage> m_fiberStorage;
};

}

#include "ewas_fiber_local_storage.inl"