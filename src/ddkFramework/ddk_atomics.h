//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <utility>
#include "ddk_template_helper.h"
#include "ddk_arena.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <type_traits>

#ifdef _WIN32

#pragma warning ( push )
#pragma warning ( disable: 4793 )

#define _WINSOCKAPI_
#include "Windows.h"

#endif

namespace ddk
{

template<typename T>
struct atomic8
{
	static_assert(sizeof(T) == 1,"You cannot operate atomically on types bigger than 8 bits");
	static_assert(std::is_trivially_copyable<T>::value &&
					std::is_copy_constructible<T>::value &&
					std::is_move_constructible<T>::value &&
					std::is_copy_assignable<T>::value &&
					std::is_move_assignable<T>::value,"You cannot apply atomic32 operation son types of non trivial data type");

	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_exchange(atomic8<TT>&, const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend bool atomic_compare_exchange(atomic8<TT>&,const TTT&,const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_compare_exchange_val(atomic8<TT>&,const TTT&,const TTT&);
	template<typename TT>
	friend TT atomic_pre_increment(atomic8<TT>&);
	template<typename TT>
	friend TT atomic_post_increment(atomic8<TT>&);
	template<typename TT>
	friend TT atomic_pre_decremeemt(atomic8<TT>&);
	template<typename TT>
	friend TT atomic_post_decrement(atomic8<TT>&);
	template<typename TT>
	friend TT atomic_add(atomic8<TT>&,const TT&);

	atomic8() = default;
	atomic8(const T& i_value);
	atomic8(T&& i_value);
	atomic8(const atomic8& other);
	atomic8(atomic8&& other);
	atomic8& operator=(const atomic8& other);
	atomic8& operator=(atomic8&& other);
	bool operator==(const atomic8& other) const;
	bool operator==(const T& other) const;
	T operator++();
	T operator++(int);
	T operator--();
	T operator--(int);
	inline const T& get() const;
	inline T& get();
	inline void set(const T& i_value);
	inline void set(T&& i_value);
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT() const;
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT();

private:
	T* _get_typed_arena();
	char* _get_arena();

	T m_value;
};

typedef atomic8<bool> atomic_bool;
typedef atomic8<char> atomic_char;
typedef atomic8<unsigned char> atomic_uchar;

template<typename T>
struct atomic32
{
	static_assert(sizeof(T) <= 4, "You cannot operate atomically on types bigger than 32 bits");
    static_assert(std::is_trivially_copyable<T>::value &&
					std::is_copy_constructible<T>::value &&
					std::is_move_constructible<T>::value &&
					std::is_copy_assignable<T>::value &&
					std::is_move_assignable<T>::value, "You cannot apply atomic32 operation son types of non trivial data type");

	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_exchange(atomic32<TT>&, const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend bool atomic_compare_exchange(atomic32<TT>&, const TTT&, const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_compare_exchange_val(atomic32<TT>&, const TTT&, const TTT&);
	template<typename TT>
	friend TT atomic_pre_increment(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_post_increment(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_pre_decremeemt(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_post_decrement(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_add(atomic32<TT>&,const TT&);

	atomic32() = default;
	atomic32(const T& i_value);
	atomic32(T&& i_value);
	atomic32(const atomic32& other);
	atomic32(atomic32&& other);
	atomic32& operator=(const atomic32& other);
	atomic32& operator=(atomic32&& other);
	bool operator==(const atomic32& other) const;
	bool operator==(const T& other) const;
	T operator++();
	T operator++(int);
	T operator--();
	T operator--(int);
	inline const T& get() const;
	inline T& get();
	inline void set(const T& i_value);
	inline void set(T&& i_value);
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT() const;
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT();

private:
	T* _get_typed_arena();
	uint32_t* _get_arena();

	T m_value;
};

//put here whatever specialization you need
typedef atomic32<int> atomic_int;
typedef atomic32<unsigned int> atomic_uint;

template<typename T>
struct atomic64
{
	static_assert(sizeof(T) <= 8, "You cannot operate atomically on types bigger than 64 bits");
    static_assert(std::is_trivially_copyable<T>::value &&
					std::is_copy_constructible<T>::value &&
					std::is_move_constructible<T>::value &&
					std::is_copy_assignable<T>::value &&
					std::is_move_assignable<T>::value, "You cannot apply atomic32 operation son types of non trivial data type");

	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_exchange(atomic64<TT>&, const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend bool atomic_compare_exchange(atomic64<TT>&, const TTT&, const TTT&);
	TEMPLATE(typename TT, typename TTT)
	REQUIRED(IS_CONSTRUCTIBLE(TT, TTT))
	friend TT atomic_compare_exchange_val(atomic64<TT>&, const TTT&, const TTT&);
	template<typename TT>
	friend TT atomic_pre_increment(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_post_increment(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_pre_decrement(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_post_decrement(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_add(atomic64<TT>&,const TT&);

	atomic64() = default;
	atomic64(const T& i_value);
	atomic64(T&& i_value);
	atomic64(const atomic64& other);
	atomic64(atomic64&& other);
	atomic64& operator=(const atomic64& other);
	atomic64& operator=(atomic64&& other);
	bool operator==(const atomic64& other) const;
	bool operator==(const T& other) const;
	T operator++();
	T operator++(int);
	T operator--();
	T operator--(int);
	inline const T& get() const;
	inline T& get();
	inline void set(const T& i_value);
	inline void set(T&& i_value);
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT() const;
	TEMPLATE(typename TT)
	REQUIRES(IS_CONVERTIBLE(T,TT))
	inline explicit operator TT();

private:
	T* _get_typed_arena();
	int64_t* _get_arena();

	T m_value;
};

typedef typename mpl::which_type<(alignof(size_t)<=4),atomic32<size_t>,atomic64<size_t>>::type  atomic_size_t;
typedef typename mpl::which_type<(alignof(uintptr_t)<=4),atomic32<uintptr_t>,atomic64<uintptr_t>>::type  atomic_uintptr_t;

template<typename T>
struct atomic : mpl::which_type<(alignof(T)<=4),atomic32<T>,atomic64<T>>::type
{
private:
	typedef typename mpl::which_type<(alignof(T)<=4),atomic32<T>,atomic64<T>>::type base_t;
public:
	using base_t::base_t;
};

template<typename T>
struct atomic<T*>
{
	template<typename TT>
	friend bool atomic_compare_exchange(atomic<TT*>&, TT*, TT*);
	template<typename TT>
	friend TT* atomic_compare_exchange_val(atomic<TT*>&, TT*, TT*);

	atomic(T* i_value);
	bool operator==(const atomic& other) const;
	bool operator==(const T* other) const;
	T operator++();
	T operator++(int);
	T operator--();
	T operator--(int);
	const T* get() const;
	T* get();
	void set(T* i_value);
	atomic_uintptr_t& as_number();
	const atomic_uintptr_t& as_number() const;

private:
	void* volatile* _get_arena();

	T* m_value;
};

TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T atomic_exchange(atomic8<T>& i_atomic, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T, TT))
inline T atomic_exchange(atomic32<T>& i_atomic, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T atomic_exchange(atomic64<T>& i_atomic, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRED(IS_BASE_OF(T,TT))
inline T* atomic_exchange(atomic<T*>& i_atomic, TT* i_desiredValue);

TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline bool atomic_compare_exchange(atomic8<T>& i_atomic,const TT& i_expectedValue,const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline bool atomic_compare_exchange(atomic32<T>& i_atomic, const TT& i_expectedValue, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline bool atomic_compare_exchange(atomic64<T>& i_atomic, const TT& i_expectedValue, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_BASE_OF(T,TT))
inline T* atomic_compare_exchange(atomic<T*>& i_atomic, TT* i_expectedValue, TT* i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T atomic_compare_exchange_val(atomic8<T>& i_atomic,const TT& i_expectedValue,const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T atomic_compare_exchange_val(atomic32<T>& i_atomic, const TT& i_expectedValue, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T atomic_compare_exchange_val(atomic64<T>& i_atomic, const TT& i_expectedValue, const TT& i_desiredValue);
TEMPLATE(typename T, typename TT)
REQUIRES(IS_CONSTRUCTIBLE(T,TT))
inline T* atomic_compare_exchange_val(atomic<T*>& i_atomic, TT* i_expectedValue, TT* i_desiredValue);
template<typename T>
inline T atomic_pre_increment(atomic8<T>& i_atomic);
template<typename T>
inline T atomic_pre_increment(atomic32<T>& i_atomic);
template<typename T>
inline T atomic_pre_increment(atomic64<T>& i_atomic);
template<typename T>
inline T atomic_post_increment(atomic8<T>& i_atomic);
template<typename T>
inline T atomic_post_increment(atomic32<T>& i_atomic);
template<typename T>
inline T atomic_post_increment(atomic64<T>& i_atomic);
template<typename T>
inline T atomic_pre_decrement(atomic8<T>& i_atomic);
template<typename T>
inline T atomic_pre_decrement(atomic32<T>& i_atomic);
template<typename T>
inline T atomic_pre_decrement(atomic64<T>& i_atomic);
template<typename T>
inline T atomic_post_decrement(atomic8<T>& i_atomic);
template<typename T>
inline T atomic_post_decrement(atomic32<T>& i_atomic);
template<typename T>
inline T atomic_post_decrement(atomic64<T>& i_atomic);

template<typename T>
inline T atomic_add(atomic8<T>& i_atomic, const T& i_value);
template<typename T>
inline T atomic_add(atomic32<T>& i_atomic, const T& i_value);
template<typename T>
inline T atomic_add(atomic64<T>& i_atomic, const T& i_value);

}

#include "ddk_atomics.inl"

#ifdef _WIN32

#pragma warning ( pop )

#endif
