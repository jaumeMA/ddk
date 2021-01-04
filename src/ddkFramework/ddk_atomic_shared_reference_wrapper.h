#pragma once

#include "ddk_atomic_shared_pointer_wrapper.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T, bool Weakable>
class atomic_shared_reference_wrapper_impl : public atomic_shared_pointer_wrapper_impl<T,Weakable>
{
public:
	using atomic_shared_pointer_wrapper_impl<T,Weakable>::operator*;
	using atomic_shared_pointer_wrapper_impl<T,Weakable>::operator->;
	using atomic_shared_pointer_wrapper_impl<T,Weakable>::get;
	using atomic_shared_pointer_wrapper_impl<T,Weakable>::clear;

	atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl& other);
	atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<T,Weakable>& other);
	atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl&& other);
	atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<T,Weakable>&& other);
	TEMPLATE(typename TT, bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl<TT,WWeakable>& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,WWeakable>& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl<TT,WWeakable>&& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable || Weakable == WWeakable)
	atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,WWeakable>&& other);
	atomic_shared_reference_wrapper_impl& operator=(const atomic_shared_reference_wrapper_impl& other);
	atomic_shared_reference_wrapper_impl& operator=(const shared_reference_wrapper_impl<T,Weakable>& other);
	atomic_shared_reference_wrapper_impl& operator=(atomic_shared_reference_wrapper_impl&& other);
	atomic_shared_reference_wrapper_impl& operator=(shared_reference_wrapper_impl<T,Weakable>&& other);
	operator shared_reference_wrapper_impl<T,Weakable>&();
	operator const shared_reference_wrapper_impl<T,Weakable>&() const;
};

}

template<typename T>
using atomic_shared_reference_wrapper = detail::atomic_shared_reference_wrapper_impl<T,true>;
template<typename T>
using atomic_distributed_reference_wrapper = detail::atomic_shared_reference_wrapper_impl<T,false>;

}

#include "ddk_atomic_shared_reference_wrapper.inl"