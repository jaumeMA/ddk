#pragma once

#include "ddk_reference_counter.h"
#include "ddk_tagged_pointer.h"
#include "ddk_container_exceptions.h"

namespace ddk
{

#ifdef DDK_DEBUG

#define THIS_OBJECT (*this)

#define REGISTER_STACK_TRACE(_OBJECT) \
	if (_OBJECT.m_refCounter) \
	{ \
		_OBJECT.m_refCounter->registerStackTrace(reinterpret_cast<size_t>(&_OBJECT)); \
	}
#define UNREGISTER_STACK_TRACE(_OBJECT) \
	if (_OBJECT.m_refCounter) \
	{ \
		_OBJECT.m_refCounter->unregisterStackTrace(reinterpret_cast<size_t>(&_OBJECT)); \
	}
#define COPY_STACK_TRACE(_OLD_OBJECT,_NEW_OBJECT) \
	if (_NEW_OBJECT.m_refCounter) \
	{ \
		_NEW_OBJECT.m_refCounter->copyStackTrace(reinterpret_cast<size_t>(&_OLD_OBJECT),reinterpret_cast<size_t>(&_NEW_OBJECT)); \
	}
#define MOVE_STACCK_TRACE(_OLD_OBJECT,_NEW_OBJECT) \
	if (_NEW_OBJECT.m_refCounter) \
	{ \
		_NEW_OBJECT.m_refCounter->reassignStackTrace(reinterpret_cast<size_t>(&_OLD_OBJECT),reinterpret_cast<size_t>(&_NEW_OBJECT)); \
	}

template<typename>
class lent_reference_wrapper;
template<typename>
class unique_pointer_wrapper;
template<typename>
class shared_pointer_wrapper;

template<typename T>
class lent_pointer_wrapper
{
	template<typename>
	friend class lent_pointer_wrapper;
	template<typename>
	friend class lent_reference_wrapper;
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const unique_pointer_wrapper<TT>& i_uniqueRef);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> dynamic_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> dynamic_lent_cast(const lent_reference_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> static_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_reference_wrapper<TTT> static_lent_cast(const lent_reference_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_pointer_wrapper<TTT> reinterpret_lent_cast(const lent_pointer_wrapper<TT>&);
	template<typename TTT, typename TT>
	friend lent_reference_wrapper<TTT> reinterpret_lent_cast(const lent_reference_wrapper<TT>&);
	template<typename TT>
	lent_pointer_wrapper<TT> const_lent_cast(const lent_pointer_wrapper<const TT>&);
	template<typename TT>
	lent_reference_wrapper<TT> const_lent_cast(const lent_reference_wrapper<const TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> promote_to_ref(const lent_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> __make_lent_pointer(TT*, const tagged_pointer<lent_reference_counter>&);

	lent_pointer_wrapper(T* i_data, const tagged_pointer<lent_reference_counter>& i_refCounter);

public:
	typedef tagged_pointer<lent_reference_counter> tagged_reference_counter;
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	lent_pointer_wrapper();
	lent_pointer_wrapper(const std::nullptr_t&);
	lent_pointer_wrapper(const lent_pointer_wrapper& other);
	lent_pointer_wrapper(lent_pointer_wrapper&& other);
	template<typename TT>
	lent_pointer_wrapper(const lent_pointer_wrapper<TT>& other);
	template<typename TT>
	lent_pointer_wrapper(lent_pointer_wrapper<TT>&& other);
	~lent_pointer_wrapper();
	lent_pointer_wrapper& operator=(const std::nullptr_t&);
	lent_pointer_wrapper& operator=(const lent_pointer_wrapper& other);
	lent_pointer_wrapper& operator=(lent_pointer_wrapper&& other);
	template<typename TT>
	lent_pointer_wrapper& operator=(const lent_pointer_wrapper<TT>& other);
	template<typename TT>
	lent_pointer_wrapper& operator=(lent_pointer_wrapper<TT>&& other);
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
	T* extract();

private:
	inline tagged_reference_counter get_reference_counter() const;

	T* m_data;
	tagged_reference_counter m_refCounter;
};

#else

template<typename T>
using lent_pointer_wrapper = T*;

#endif

}

#include "ddk_lent_pointer_wrapper.inl"