#pragma once

#include "ddk_reference_counter.h"
#include "ddk_smart_ptr_template_helper.h"
#include "ddk_shared_pointer_wrapper.h"

namespace ddk
{

template<typename>
class weak_pointer_wrapper;

namespace detail
{

template<typename TT>
weak_pointer_wrapper<TT> __make_weak_pointer(TT*,const tagged_pointer<shared_reference_counter>&,const IReferenceWrapperDeleter*);

}

template<typename T>
class weak_pointer_wrapper
{
	template<typename>
	friend class weak_pointer_wrapper;
	template<typename TT>
	friend weak_pointer_wrapper<TT> detail::__make_weak_pointer(TT*,const tagged_pointer<shared_reference_counter>&,const IReferenceWrapperDeleter*);
	template<typename TT>
	friend shared_pointer_wrapper<const TT> share(const weak_pointer_wrapper<TT>&);
	template<typename TT>
	friend shared_pointer_wrapper<TT> share(weak_pointer_wrapper<TT>&);

public:
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	weak_pointer_wrapper() = default;
	weak_pointer_wrapper(const weak_pointer_wrapper& other);
	weak_pointer_wrapper(weak_pointer_wrapper&& other);
	template<typename TT>
	weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other);
	template<typename TT>
	weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other);
	~weak_pointer_wrapper();

	weak_pointer_wrapper& operator=(std::nullptr_t);
	weak_pointer_wrapper& operator=(const weak_pointer_wrapper& other);
	weak_pointer_wrapper& operator=(weak_pointer_wrapper&& other);
	template<typename TT>
	weak_pointer_wrapper& operator=(const weak_pointer_wrapper<TT>& other);
	template<typename TT>
	weak_pointer_wrapper& operator=(weak_pointer_wrapper<TT>&& other);

	bool operator==(const std::nullptr_t&) const;
	bool operator!=(const std::nullptr_t&) const;
	shared_pointer_wrapper<T> share() const;

private:
	weak_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter,const IReferenceWrapperDeleter* i_deleter);
	void clearIfCounterVoid(size_t i_numWeakRefs);

	T* m_data = nullptr;
	mutable tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter = nullptr;
};

}

#include "ddk_weak_pointer_wrapper.inl"