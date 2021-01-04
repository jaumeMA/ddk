#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_spin_lock.h"
#include "ddk_concepts.h"

namespace ddk
{

namespace detail
{

template<typename T, bool Weakable>
class atomic_shared_pointer_wrapper_impl
{
public:
	atomic_shared_pointer_wrapper_impl() = default;
	atomic_shared_pointer_wrapper_impl(std::nullptr_t);
	atomic_shared_pointer_wrapper_impl(T* i_data,IReferenceWrapperDeleter* i_refDeleter = nullptr);
	atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl& other);
	atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<T,Weakable>& other);
	atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl&& other);
	atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<T,Weakable>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl<TT,Weakable>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,Weakable>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl<TT,Weakable>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,Weakable>&& other);
	~atomic_shared_pointer_wrapper_impl();
	atomic_shared_pointer_wrapper_impl& operator=(std::nullptr_t);
	atomic_shared_pointer_wrapper_impl& operator=(const atomic_shared_pointer_wrapper_impl& other);
	atomic_shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<T,Weakable>& other);
	atomic_shared_pointer_wrapper_impl& operator=(atomic_shared_pointer_wrapper_impl&& other);
	atomic_shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<T,Weakable>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(const atomic_shared_pointer_wrapper_impl<TT,Weakable>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<TT,Weakable>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(atomic_shared_pointer_wrapper_impl<TT,Weakable>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<TT,Weakable>&& other);
	inline operator shared_pointer_wrapper_impl<T,Weakable>&();
	inline operator const shared_pointer_wrapper_impl<T,Weakable>&() const;
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
	shared_pointer_wrapper_impl<T,Weakable> m_ptr;
};

}

template<typename T>
using atomic_shared_pointer_wrapper = detail::atomic_shared_pointer_wrapper_impl<T,true>;
template<typename T>
using atomic_distributed_pointer_wrapper = detail::atomic_shared_pointer_wrapper_impl<T,false>;

}

#include "ddk_atomic_shared_pointer_wrapper.inl"