#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include <utility>
#include "ddk_tagged_pointer.h"
#include "ddk_lent_reference_wrapper.h"
#include <type_traits>

#define ENSURE_SCOPE_LIFETIME(_PTR) const auto ___##_PTR = _PTR;

namespace ddk
{

template<typename T>
class shared_pointer_wrapper
{
	template<typename>
	friend class shared_pointer_wrapper;
	template<typename>
	friend class shared_reference_wrapper;
	template<typename>
	friend class weak_pointer_wrapper;
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> static_shared_cast(const shared_pointer_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_reference_wrapper<TT> static_shared_cast(const shared_reference_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> dynamic_shared_cast(const shared_pointer_wrapper<TTT>&);
	template<typename TT, typename TTT>
	friend shared_pointer_wrapper<TT> reinterpret_shared_cast(const shared_pointer_wrapper<TTT>&);
	template<typename TT,typename TTT>
	friend shared_pointer_wrapper<TT> dynamic_shared_cast(const shared_reference_wrapper<TTT>&);
	template<typename TT, typename TTT>
	friend shared_pointer_wrapper<TT> reinterpret_shared_cast(const shared_reference_wrapper<TTT>&);
	template<typename TT>
	friend shared_reference_wrapper<TT> const_shared_cast(const shared_reference_wrapper<const TT>&);
	template<typename TT>
	friend shared_pointer_wrapper<TT> const_shared_cast(const shared_pointer_wrapper<const TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(shared_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const shared_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(shared_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const shared_reference_wrapper<TT>&);
	template<typename TT>
	friend shared_reference_wrapper<TT> promote_to_ref(const shared_pointer_wrapper<TT>&);
	template<typename TT>
	friend shared_pointer_wrapper<TT> __make_shared_pointer(TT* i_data, const tagged_pointer<shared_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
	template<typename TT>
	friend weak_pointer_wrapper<TT> weak(const shared_pointer_wrapper<TT>&);

public:
	typedef tagged_pointer<shared_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	shared_pointer_wrapper();
	shared_pointer_wrapper(std::nullptr_t);
	shared_pointer_wrapper(T* i_data, IReferenceWrapperDeleter* i_refDeleter = nullptr);
	shared_pointer_wrapper(const shared_pointer_wrapper& other);
	shared_pointer_wrapper(shared_pointer_wrapper&& other);
	template<typename TT>
	shared_pointer_wrapper(const shared_pointer_wrapper<TT>& other);
	template<typename TT>
	shared_pointer_wrapper(shared_pointer_wrapper<TT>&& other);
	~shared_pointer_wrapper();
	shared_pointer_wrapper& operator=(std::nullptr_t);
	shared_pointer_wrapper& operator=(const shared_pointer_wrapper& other);
	shared_pointer_wrapper& operator=(shared_pointer_wrapper&& other);
	template<typename TT>
	shared_pointer_wrapper& operator=(const shared_pointer_wrapper<TT>& other);
	template<typename TT>
	shared_pointer_wrapper& operator=(shared_pointer_wrapper<TT>&& other);
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
	shared_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr, bool i_alreadyIncremented = false);
	void clearIfCounterVoid(size_t i_currNumRefs);

	T* m_data;
	tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter;
};

}

#include "ddk_shared_pointer_wrapper.inl"