//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_system_allocator.h"

namespace ddk
{

template<typename T, typename Allocator>
class thread_local_storage
{
public:
	thread_local_storage() = default;
	thread_local_storage(thread_local_storage&& other);
	~thread_local_storage();
	template<typename ... Args>
	inline T* construct(Args&& ... i_args);
	inline void destroy();
	template<typename ... Args>
	inline T* assign(Args&& ... i_args);
	auto get();
	auto get() const;
	T* get_address();
	const T* get_address() const;
	T extract() &&;
	bool empty() const;

private:
	Allocator m_alloc;
	void* m_address = nullptr;
};

}

#include "ddk_thread_local_storage.inl"
