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

template<typename T, bool Weakable>
class shared_pointer_wrapper_impl
{
	template<typename,bool>
	friend class shared_pointer_wrapper_impl;
	template<typename,bool>
	friend class shared_reference_wrapper_impl;
	template<typename>
	friend class ddk::weak_pointer_wrapper;
	template<typename TT,typename TTT, bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __static_shared_cast(const shared_pointer_wrapper_impl<TTT,WWeakable>&);
	template<typename TT,typename TTT, bool WWeakable>
	friend shared_reference_wrapper_impl<TT,WWeakable> __static_shared_cast(const shared_reference_wrapper_impl<TTT,WWeakable>&);
	template<typename TT,typename TTT,bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __dynamic_shared_cast(const shared_pointer_wrapper_impl<TTT,WWeakable>&);
	template<typename TT, typename TTT,bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __reinterpret_shared_cast(const shared_pointer_wrapper_impl<TTT,WWeakable>&);
	template<typename TT, typename TTT,bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __reinterpret_shared_cast(const shared_reference_wrapper_impl<TTT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend shared_reference_wrapper_impl<TT,WWeakable> __const_shared_cast(const shared_reference_wrapper_impl<const TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __const_shared_cast(const shared_pointer_wrapper_impl<const TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend lent_pointer_wrapper<TT> __lend(shared_pointer_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend lent_pointer_wrapper<TT> __lend(const shared_pointer_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend lent_reference_wrapper<TT> __lend(shared_reference_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend lent_reference_wrapper<TT> __lend(const shared_reference_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend shared_reference_wrapper_impl<TT,WWeakable> __promote_to_ref(const shared_pointer_wrapper_impl<TT,WWeakable>&);
	template<typename TT,bool WWeakable>
	friend shared_pointer_wrapper_impl<TT,WWeakable> __make_shared_pointer(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<TT> __weak(shared_pointer_wrapper_impl<TT,true>&);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<const TT> __weak(const shared_pointer_wrapper_impl<TT,true>&);

public:
	typedef typename mpl::static_if<Weakable,shared_reference_counter,distributed_reference_counter>::type reference_counter;
	typedef tagged_pointer<reference_counter> tagged_reference_counter;
	typedef T nested_type;

	shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl(std::nullptr_t);
	shared_pointer_wrapper_impl(T* i_data, IReferenceWrapperDeleter* i_refDeleter = nullptr);
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl&& other);
	TEMPLATE(typename TT, bool WWeakable)
	REQUIRES_COND(Weakable == false || Weakable == WWeakable)
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,WWeakable>& other);
	TEMPLATE(typename TT, bool WWeakable)
	REQUIRES_COND(Weakable == false || Weakable == WWeakable)
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,WWeakable>&& other);
	~shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl& operator=(std::nullptr_t);
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl&& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable == false || Weakable == WWeakable)
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<TT,WWeakable>& other);
	TEMPLATE(typename TT,bool WWeakable)
	REQUIRES_COND(Weakable == false || Weakable == WWeakable)
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<TT,WWeakable>&& other);
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
using shared_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,true>;
template<typename T>
using distributed_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,false>;

}

#include "ddk_shared_pointer_wrapper.inl"