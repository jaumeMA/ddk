
#include "ddk_macros.h"

namespace ddk
{

template<typename T, typename Allocator>
thread_local_storage<T,Allocator>::thread_local_storage(thread_local_storage&& other)
: m_address(nullptr)
{
	std::swap(m_address,other.m_address);
}
template<typename T,typename Allocator>
thread_local_storage<T,Allocator>::~thread_local_storage()
{
	if (m_address)
	{
		destroy();
	}
}
template<typename T, typename Allocator>
bool thread_local_storage<T,Allocator>::empty() const
{
	return m_address == nullptr;
}
template<typename T, typename Allocator>
template<typename ... Args>
T* thread_local_storage<T,Allocator>::construct(Args&& ... i_args)
{
	DDK_ASSERT(m_address==nullptr, "Constructing already constructed address");

	if((m_address = m_alloc.allocate(sizeof(T))))
	{
		new(m_address) T(std::forward<Args>(i_args) ...);

		return reinterpret_cast<T*>(m_address);
	}
	else
	{
		DDK_FAIL("Could not allocate address");

		return nullptr;
	}
}
template<typename T, typename Allocator>
void thread_local_storage<T,Allocator>::destroy()
{
	DDK_ASSERT(m_address!=nullptr, "Destroying null address");

	if(T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		typedAddress->~T();

		m_alloc.deallocate(m_address);

		m_address = nullptr;
	}
}
template<typename T, typename Allocator>
template<typename ... Args>
T* thread_local_storage<T,Allocator>::assign(Args&& ... i_args)
{
	if (T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		*typedAddress = T(std::forward<Args>(i_args) ...);

		return typedAddress;
	}
	else
	{
		if ((m_address = m_alloc.allocate(sizeof(T))))
		{
			new(m_address) T(std::forward<Args>(i_args) ...);

			return reinterpret_cast<T*>(m_address);
		}
		else
		{
			DDK_FAIL("Could not allocate address");

			return nullptr;
		}
	}
}
template<typename T, typename Allocator>
T& thread_local_storage<T,Allocator>::get()
{
	return *reinterpret_cast<T*>(m_address);
}
template<typename T,typename Allocator>
const T& thread_local_storage<T,Allocator>::get() const
{
	return *reinterpret_cast<T*>(m_address);
}
template<typename T, typename Allocator>
T thread_local_storage<T,Allocator>::extract() &&
{
	if(T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		T res = std::move(*typedAddress);

		typedAddress->~T();

		m_alloc.deallocate(m_address);

		m_address = nullptr;

		return std::move(res);
	}
	else
	{
		DDK_FAIL("Trying to extract empty thread local storage");

		return crash_on_return<T>::value();
	}
}
template<typename T, typename Allocator>
T* thread_local_storage<T,Allocator>::get_address()
{
	return reinterpret_cast<T*>(m_address);
}
template<typename T,typename Allocator>
const T* thread_local_storage<T,Allocator>::get_address() const
{
	return reinterpret_cast<T*>(m_address);
}

}