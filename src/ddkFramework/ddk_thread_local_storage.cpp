#include "ewas_thread_local_storage.h"

namespace ewas
{

thread_local_storage::thread_local_storage(thread_local_storage&& other)
: m_address(nullptr)
{
	std::swap(m_address,other.m_address);
}
thread_local_storage::~thread_local_storage()
{
	if(m_address)
	{
		m_alloc.deallocate(m_address);
	}
}
bool thread_local_storage::empty() const
{
	return m_address == nullptr;
}

}