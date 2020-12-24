#pragma once

#include "ddk_atomic_shared_pointer_wrapper.h"

namespace ddk
{

template<typename T>
class atomic_shared_reference_wrapper : public atomic_shared_pointer_wrapper<T>
{
public:
	using atomic_shared_pointer_wrapper<T>::operator*;
	using atomic_shared_pointer_wrapper<T>::operator->;
	using atomic_shared_pointer_wrapper<T>::get;
	using atomic_shared_pointer_wrapper<T>::clear;

	atomic_shared_reference_wrapper(const atomic_shared_reference_wrapper<T>& other);
	atomic_shared_reference_wrapper(const shared_reference_wrapper<T>& other);
	atomic_shared_reference_wrapper(atomic_shared_reference_wrapper<T>&& other);
	atomic_shared_reference_wrapper(shared_reference_wrapper<T>&& other);
	template<typename TT>
	atomic_shared_reference_wrapper(const atomic_shared_reference_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_reference_wrapper(const shared_reference_wrapper<TT>& other);
	template<typename TT>
	atomic_shared_reference_wrapper(atomic_shared_reference_wrapper<TT>&& other);
	template<typename TT>
	atomic_shared_reference_wrapper(shared_reference_wrapper<TT>&& other);
	atomic_shared_reference_wrapper& operator=(const atomic_shared_reference_wrapper<T>& other);
	atomic_shared_reference_wrapper& operator=(const shared_reference_wrapper<T>& other);
	atomic_shared_reference_wrapper& operator=(atomic_shared_reference_wrapper<T>&& other);
	atomic_shared_reference_wrapper& operator=(shared_reference_wrapper<T>&& other);
	operator shared_reference_wrapper<T>() const;
};

}

#include "ddk_atomic_shared_reference_wrapper.inl"