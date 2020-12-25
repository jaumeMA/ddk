#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_spin_lock.h"

namespace ddk
{

template<typename>
class atomic_weak_pointer_wrapper;

template<typename T>
class atomic_shared_pointer_wrapper
{
	template<typename TT>
	friend atomic_weak_pointer_wrapper<TT> weak(atomic_shared_pointer_wrapper<TT>);

public:
	atomic_shared_pointer_wrapper() = default;
	atomic_shared_pointer_wrapper(std::nullptr_t);
	atomic_shared_pointer_wrapper(T* i_data,IReferenceWrapperDeleter* i_refDeleter = nullptr);
	atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper& other);
	atomic_shared_pointer_wrapper(const shared_pointer_wrapper<T>& other);
	atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper&& other);
	atomic_shared_pointer_wrapper(shared_pointer_wrapper<T>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper(const atomic_shared_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper(const shared_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper(atomic_shared_pointer_wrapper<TT>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper(shared_pointer_wrapper<TT>&& other);
	~atomic_shared_pointer_wrapper();
	atomic_shared_pointer_wrapper& operator=(std::nullptr_t);
	atomic_shared_pointer_wrapper& operator=(const atomic_shared_pointer_wrapper& other);
	atomic_shared_pointer_wrapper& operator=(const shared_pointer_wrapper<T>& other);
	atomic_shared_pointer_wrapper& operator=(atomic_shared_pointer_wrapper&& other);
	atomic_shared_pointer_wrapper& operator=(shared_pointer_wrapper<T>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper& operator=(const atomic_shared_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper& operator=(const shared_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper& operator=(atomic_shared_pointer_wrapper<TT>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper& operator=(shared_pointer_wrapper<TT>&& other);
	inline operator shared_pointer_wrapper<T>() const;
	inline bool operator==(std::nullptr_t) const;
	inline bool operator!=(std::nullptr_t) const;
	inline T* operator->();
	inline const T* operator->() const;
	inline T& operator*();
	inline const T& operator*() const;
	inline operator bool() const;
	void clear();
	inline T* get();
	inline const T* get() const;
	inline const IReferenceWrapperDeleter* get_deleter() const;
	inline bool empty() const;

private:
	mutable spin_lock m_barrier;
	shared_pointer_wrapper<T> m_ptr;
};

}

#include "ddk_atomic_shared_pointer_wrapper.inl"