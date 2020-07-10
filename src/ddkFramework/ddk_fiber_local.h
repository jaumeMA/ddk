#pragma once

#include "ewas_fiber_local_storage.h"

namespace ewas
{

template<typename T, typename Tag>
class fiberlocal
{
public:
	fiberlocal() = default;
	fiberlocal(const fiberlocal&) = delete;
	fiberlocal(fiberlocal&&) = default;
	~fiberlocal() = default;

	template<typename ... Args>
	inline T& acquire(Args&& ... i_args);
	inline T& get();
	inline void clear();
	inline bool empty() const;

private:
	inline fiber_local_storage& get_address() const;
};

}

#include "ewas_fiber_local.inl"