#pragma once

#include "ddk_thread_local_storage.h"

namespace ddk
{

template<typename T, typename Tag = T, typename Allocator = system_allocator>
class threadlocal
{
	static const size_t k_typeSize = mpl::size_of_qualified_type<T>::value;

public:
	threadlocal() = default;
	threadlocal(const threadlocal&) = delete;
	threadlocal(threadlocal&&) = default;
	template<typename ... Args>
	threadlocal(Args&& ... i_args);
	~threadlocal() = default;

	template<typename ... Args>
	T& acquire(Args&& ... i_args);
	template<typename ... Args>
	T& set(Args&& ... i_args);
	T& get();
	const T& get() const;
	T* get_ptr();
	const T* get_ptr() const;
	T extract() &&;
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;
	bool empty() const;
	void clear();

private:
	inline thread_local_storage<T,Allocator>& get_address() const;
};

}

#include "ddk_thread_local.inl"
