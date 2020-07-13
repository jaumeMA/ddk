#pragma once

#include "ddk_system_allocator.h"

namespace ddk
{

class thread_local_storage
{
public:
	thread_local_storage() = default;
	thread_local_storage(thread_local_storage&& other);
	~thread_local_storage();
	template<typename T, typename ... Args>
	inline T* construct(Args&& ... i_args);
	template<typename T>
	inline void destroy();
	template<typename T>
	T& get();
	template<typename T>
	T* get_address();
	bool empty() const;

private:
	system_allocator m_alloc;
	void* m_address = nullptr;
};

}

#include "ddk_thread_local_storage.inl"
