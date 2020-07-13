#pragma once

#include <unordered_map>
#include "ddk_fiber_defs.h"
#include "ddk_thread_local_storage.h"

namespace ddk
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

#include "ddk_fiber_local_storage.inl"
