#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_tagged_pointer.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_concepts.h"
#include <utility>
#include <type_traits>

#define ENSURE_SCOPE_LIFETIME(_PTR) const auto ___##_PTR = _PTR;

namespace ddk
{

template<typename>
class weak_pointer_wrapper;

namespace detail
{

template<typename T, typename ReferenceCounter>
class shared_pointer_wrapper_impl
{
	template<typename,typename>
	friend class shared_pointer_wrapper_impl;
	template<typename,typename>
	friend class shared_reference_wrapper_impl;
	template<typename>
	friend class ddk::weak_pointer_wrapper;
	template<typename TT,typename TTT, typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __static_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename TTT, typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __static_shared_cast(const shared_reference_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __dynamic_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT, typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __reinterpret_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT, typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __reinterpret_shared_cast(const shared_reference_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __const_shared_cast(const shared_reference_wrapper_impl<const TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __const_shared_cast(const shared_pointer_wrapper_impl<const TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_pointer_wrapper<TT> __lend(shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_pointer_wrapper<TT> __lend(const shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_reference_wrapper<TT> __lend(shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_reference_wrapper<TT> __lend(const shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __promote_to_ref(const shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __make_shared_pointer(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<TT> __weak(shared_pointer_wrapper_impl<TT,shared_reference_counter>&);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<const TT> __weak(const shared_pointer_wrapper_impl<TT,shared_reference_counter>&);

public:
	typedef tagged_pointer<ReferenceCounter> tagged_reference_counter;
	typedef T nested_type;

	shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl(std::nullptr_t);
	shared_pointer_wrapper_impl(T* i_data, IReferenceWrapperDeleter* i_refDeleter = nullptr);
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl&& other);
	template<typename TT>
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	~shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl& operator=(std::nullptr_t);
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl&& other);
	template<typename TT>
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
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
	shared_pointer_wrapper_impl(T* i_data, const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr, bool i_alreadyIncremented = false);
	void clearIfCounterVoid(size_t i_currNumRefs);

	T* m_data;
	tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter;
};

}

template<typename T>
using shared_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,shared_reference_counter>;
template<typename T>
using distributed_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,distributed_reference_counter>;

}

#include "ddk_shared_pointer_wrapper.inl"