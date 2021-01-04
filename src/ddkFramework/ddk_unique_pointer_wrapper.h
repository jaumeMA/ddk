#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_tagged_pointer.h"
#include "ddk_reference_tag.h"
#include <type_traits>
#include <utility>

namespace ddk
{

template<typename>
class unique_pointer_wrapper;
template<typename>
class unique_reference_wrapper;

namespace detail
{

template<typename TT>
inline unique_pointer_wrapper<TT> __make_unique_pointer(TT* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter);
template<typename TT>
inline unique_pointer_wrapper<TT> __make_unique_pointer(TT* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const IReferenceWrapperDeleter* i_refDeleter);

}

template<typename T>
class unique_pointer_wrapper
{
	template<typename>
	friend class unique_pointer_wrapper;
	template<typename>
	friend class unique_reference_wrapper;

	template<typename TTT, typename TT>
	friend unique_reference_wrapper<TTT> static_unique_cast(unique_reference_wrapper<TT>);
	template<typename TTT, typename TT>
	friend unique_pointer_wrapper<TTT> static_unique_cast(unique_pointer_wrapper<TT>);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_pointer_wrapper<TT>&&);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_reference_wrapper<TT>&&);
	template<typename TT>
	friend unique_reference_wrapper<TT> const_unique_cast(unique_reference_wrapper<const TT>);
	template<typename TT>
	friend unique_pointer_wrapper<TT> const_unique_cast(unique_pointer_wrapper<const TT>);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(unique_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const unique_reference_wrapper<TT>&);
	template<typename TT>
	friend unique_reference_wrapper<TT> promote_to_ref(unique_pointer_wrapper<TT>);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> detail::__make_unique_pointer(TT* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> detail::__make_unique_pointer(TT* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter);

public:
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
	typedef T nested_type;

	unique_pointer_wrapper(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper& operator=(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper();
	unique_pointer_wrapper(std::nullptr_t);
	unique_pointer_wrapper(unique_pointer_wrapper&& other);
	template<typename TT>
	unique_pointer_wrapper(unique_pointer_wrapper<TT>&& other);
	~unique_pointer_wrapper();
	unique_pointer_wrapper& operator=(std::nullptr_t);
	unique_pointer_wrapper& operator=(unique_pointer_wrapper&& other);
	template<typename TT>
	unique_pointer_wrapper& operator=(unique_pointer_wrapper<TT>&& other);
	inline bool operator==(std::nullptr_t) const;
	inline bool operator!=(std::nullptr_t) const;
	inline T* operator->();
	inline const T* operator->() const;
	inline T& operator*();
	inline const T& operator*() const;
	inline operator bool() const;
	inline void clear();
	inline T* get();
	inline const T* get() const;
	inline bool empty() const;
	inline const IReferenceWrapperDeleter* get_deleter() const;

private:
	unique_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr);
	unique_pointer_wrapper(T* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const IReferenceWrapperDeleter* i_refDeleter = nullptr);
	void clearIfCounterVoid(bool i_hasRefs);
	inline unique_reference_counter* extract_reference_counter();

	T* m_data;
	tagged_reference_counter m_refCounter;
	const IReferenceWrapperDeleter* m_deleter;
};

namespace mpl
{

template<typename T>
struct is_copy_constructible<unique_pointer_wrapper<T>>
{
    static const bool value = false;
};

}
}

#include "ddk_unique_pointer_wrapper.inl"