
#include "ddk_macros.h"

namespace ddk
{

template<typename T, typename ... Args>
T* thread_local_storage::construct(Args&& ... i_args)
{
	EWAS_ASSERT(m_address==nullptr, "Constructing already constructed address");

	if((m_address = m_alloc.allocate(1,sizeof(T))))
	{
		new(m_address) T(std::forward<Args>(i_args) ...);

		return reinterpret_cast<T*>(m_address);
	}
	else
	{
		EWAS_FAIL("Could not allocate address");

		return nullptr;
	}
}
template<typename T>
void thread_local_storage::destroy()
{
	EWAS_ASSERT(m_address!=nullptr, "Destroying null address");

	if(T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		typedAddress->~T();

		m_alloc.deallocate(m_address);

		m_address = nullptr;
	}
}
template<typename T>
T& thread_local_storage::get()
{
	return *reinterpret_cast<T*>(m_address);
}
template<typename T>
T* thread_local_storage::get_address()
{
	return reinterpret_cast<T*>(m_address);
}

}