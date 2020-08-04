#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>
#include "ddk_template_helper.h"
#include "ddk_arena.h"

#ifdef _WIN32

#pragma warning ( push )
#pragma warning ( disable: 4793 )

#define _WINSOCKAPI_
#include "Windows.h"

#endif

namespace ddk
{

template<typename T>
struct atomic32
{
	static_assert(sizeof(T) <= 4, "You cannot operate atomically on types bigger than 32 bits");
    static_assert(std::is_trivially_copyable<T>::value &&
					std::is_copy_constructible<T>::value &&
					std::is_move_constructible<T>::value &&
					std::is_copy_assignable<T>::value &&
					std::is_move_assignable<T>::value, "You cannot apply atomic32 operation son types of non trivial data type");

	template<typename TT>
	friend bool atomic_compare_exchange(atomic32<TT>&, const TT&, const TT&);
	template<typename TT>
	friend TT atomic_compare_exchange_val(atomic32<TT>&, const TT&, const TT&);
	template<typename TT>
	friend TT atomic_pre_increment(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_post_increment(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_pre_decremeemt(atomic32<TT>&);
	template<typename TT>
	friend TT atomic_post_decrement(atomic32<TT>&);

	atomic32()
	{
	}
	atomic32(const T& i_value)
	{
		m_arena.template construct<T>(i_value);
	}
	atomic32(T&& i_value)
	{
		m_arena.template construct<T>(std::move(i_value));
	}
	atomic32(const atomic32& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template construct<T>(other.m_arena.template get<T>());
		}
	}
	atomic32(atomic32&& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template construct<T>(other.m_arena.template extract<T>());
		}
	}
	~atomic32()
	{
		m_arena.template destroy<T>();
	}
	atomic32& operator=(const atomic32& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template set_value<T>(other.m_arena.template get<T>());
		}
		else
		{
			m_arena.template destroy<T>();
		}

		return *this;
	}
	atomic32& operator=(atomic32&& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template set_value<T>(other.m_arena.template extract<T>());
		}
		else
		{
			m_arena.template destroy<T>();
		}

		return *this;
	}
	inline bool empty() const
	{
		return m_arena.empty();
	}
	inline const T& get() const
	{
		return m_arena.template get<T>();
	}
	inline T& get()
	{
		return m_arena.template get<T>();
	}
	inline void set(const T& i_value)
	{
		m_arena.template set_value<T>(i_value);
	}
	inline void set(T&& i_value)
	{
		m_arena.template set_value<T>(std::move(i_value));
	}
	inline void reset()
	{
		m_arena.template destroy<T>();
	}

private:
	T* _get_typed_arena()
	{
		return m_arena.template get_ptr<T>();
	}
	int32_t* _get_arena()
	{
		return m_arena.template get_ptr<int32_t>();
	}

	arena<4,std::alignment_of<T>::value> m_arena;
};

//put here whatever specialization you need
typedef atomic32<int> atomic_int;
typedef atomic32<bool> atomic_bool;

template<typename T>
struct atomic64
{
	static_assert(sizeof(T) <= 8, "You cannot operate atomically on types bigger than 64 bits");
    static_assert(std::is_trivially_copyable<T>::value &&
					std::is_copy_constructible<T>::value &&
					std::is_move_constructible<T>::value &&
					std::is_copy_assignable<T>::value &&
					std::is_move_assignable<T>::value, "You cannot apply atomic32 operation son types of non trivial data type");

	template<typename TT>
	friend bool atomic_compare_exchange(atomic64<TT>&, const TT&, const TT&);
	template<typename TT>
	friend TT atomic_compare_exchange_val(atomic64<TT>&, const TT&, const TT&);
	template<typename TT>
	friend TT atomic_pre_increment(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_post_increment(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_pre_decrement(atomic64<TT>&);
	template<typename TT>
	friend TT atomic_post_decrement(atomic64<TT>&);

	atomic64()
	{
	}
	atomic64(const T& i_value)
	{
		m_arena.template construct<T>(i_value);
	}
	atomic64(T&& i_value)
	{
		m_arena.template construct<T>(std::move(i_value));
	}
	atomic64(const atomic64& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template construct<T>(other.m_arena.template get<T>());
		}
	}
	atomic64(atomic64&& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template construct<T>(other.m_arena.template extract<T>());
		}
	}
	~atomic64()
	{
		m_arena.template destroy<T>();
	}
	atomic64& operator=(const atomic64& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template set_value<T>(other.m_arena.template get<T>());
		}
		else
		{
			m_arena.template destroy<T>();
		}

		return *this;
	}
	atomic64& operator=(atomic64&& other)
	{
		if(other.m_arena.empty() == false)
		{
			m_arena.template set_value<T>(other.m_arena.template extract<T>());
		}
		else
		{
			m_arena.template destroy<T>();
		}

		return *this;
	}
	inline bool empty() const
	{
		return m_arena.empty();
	}
	inline const T& get() const
	{
		return m_arena.template get<T>();
	}
	inline T& get()
	{
		return m_arena.template get<T>();
	}
	inline void set(const T& i_value)
	{
		m_arena.template set_value<T>(i_value);
	}
	inline void set(T&& i_value)
	{
		m_arena.template set_value<T>(std::move(i_value));
	}
	inline void reset()
	{
		m_arena.template destroy<T>();
	}

private:
	T* _get_typed_arena()
	{
		return m_arena.template get_ptr<T>();
	}
	int64_t* _get_arena()
	{
		return m_arena.template get_ptr<int64_t>();
	}

	arena<8,std::alignment_of<T>::value> m_arena;
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

	atomic(T* i_value)
	{
		memcpy(&m_arena,&i_value,sizeof(T*));
	}
	const T* get() const
	{
		return m_arena;
	}
	T* get()
	{
		return m_arena;
	}
	void set(T* i_value)
	{
		memcpy(&m_arena,&i_value,sizeof(T*));
	}
	atomic_uintptr_t& as_number()
	{
		return reinterpret_cast<atomic_uintptr_t&>(*this);
	}
	const atomic_uintptr_t& as_number() const
	{
		return reinterpret_cast<const atomic_uintptr_t&>(*this);
	}

private:
	void* volatile* _get_arena()
	{
		return reinterpret_cast<void* volatile*>(&m_arena);
	}

	T* 	m_arena;
};

template<typename T>
inline bool atomic_compare_exchange(atomic32<T>& i_atomic, const T& i_expectedValue, const T& i_desiredValue)
{
#if defined(WIN32)
	return InterlockedCompareExchange(reinterpret_cast<LONG*>(i_atomic._get_arena()),i_desiredValue,i_expectedValue) == i_expectedValue;
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline bool atomic_compare_exchange(atomic64<T>& i_atomic, const T& i_expectedValue, const T& i_desiredValue)
{
#ifdef _WIN32
	return InterlockedCompareExchange64(i_atomic._get_arena(),i_desiredValue,i_expectedValue) == i_expectedValue;
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline bool atomic_compare_exchange(atomic<T*>& i_atomic, T* i_expectedValue, T* i_desiredValue)
{
#ifdef _WIN32
	return InterlockedCompareExchangePointer(i_atomic._get_arena(),i_desiredValue,i_expectedValue) == i_expectedValue;
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_bool_compare_and_swap(i_atomic.m_arena,i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_compare_exchange_val(atomic32<T>& i_atomic, const T& i_expectedValue, const T& i_desiredValue)
{
#ifdef _WIN32
	return T(InterlockedCompareExchange(reinterpret_cast<LONG*>(i_atomic._get_arena()),i_desiredValue,i_expectedValue));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_compare_exchange_val(atomic64<T>& i_atomic, const T& i_expectedValue, const T& i_desiredValue)
{
#ifdef _WIN32
	return T(InterlockedCompareExchange64(i_atomic._get_arena(),i_desiredValue,i_expectedValue));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic._get_typed_arena(),i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T* atomic_compare_exchange_val(atomic<T*>& i_atomic, T* i_expectedValue, T* i_desiredValue)
{
#ifdef _WIN32
	return T(InterlockedCompareExchangePointer(i_atomic._get_arena(),i_desiredValue,i_expectedValue));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_val_compare_and_swap(i_atomic.m_arena,i_expectedValue,i_desiredValue);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_pre_increment(atomic32<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedIncrement(reinterpret_cast<LONG*>(i_atomic._get_arena())) - 1);
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_add(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_pre_increment(atomic64<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedIncrement64(i_atomic._get_arena()) - 1);
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_add(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_post_increment(atomic32<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedIncrement(reinterpret_cast<LONG*>(i_atomic._get_arena())));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_add_and_fetch(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_post_increment(atomic64<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedIncrement64(i_atomic._get_arena()));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_add_and_fetch(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_pre_decrement(atomic32<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedDecrement(reinterpret_cast<LONG*>(i_atomic._get_arena())) + 1);
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_sub(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_pre_decrement(atomic64<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedDecrement64(i_atomic._get_arena()) + 1);
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_fetch_and_sub(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_post_decrement(atomic32<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedDecrement(reinterpret_cast<LONG*>(i_atomic._get_arena())));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_sub_and_fetch(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}
template<typename T>
inline T atomic_post_decrement(atomic64<T>& i_atomic)
{
#ifdef _WIN32
	return T(InterlockedDecrement64(i_atomic._get_arena()));
#elif defined(__LINUX__) or defined(__APPLE__)
	return __sync_sub_and_fetch(i_atomic._get_typed_arena(),1);
#else
	#error "Unsupported platform"
#endif
}

}

#ifdef _WIN32

#pragma warning ( pop )

#endif
