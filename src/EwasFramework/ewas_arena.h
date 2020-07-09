#pragma once

#include <type_traits>
#include <cstddef>
#include "embedded_type.h"

namespace ewas
{

template<size_t ArenaSize, size_t ArenaAlignment>
class arena
{
	//in case we can, take benefit of 1 byte of alignment for boolean data
  static const size_t s_actualAlignment = ArenaAlignment;

public:
	arena();
	arena(const arena&) = delete;
	~arena();
	arena& operator=(const arena&) = delete;
	template<typename T, typename ... Args>
	void construct(Args&& ... i_args);
	template<typename T, typename ... Args>
	void assign(Args&& ... i_args);
	template<typename T, typename ... Args>
	void set_value(Args&& ... i_args);
	template<typename T>
	void destroy();
	template<typename T>
	typename embedded_type<T>::cref_type get() const;
	template<typename T>
	typename embedded_type<T>::ref_type get();
	template<typename T>
	embedded_type<T> extract();
	template<typename T>
	typename embedded_type<T>::cpointer_type get_ptr() const;
	template<typename T>
	typename embedded_type<T>::pointer_type get_ptr();
	inline bool empty() const;
	template<typename T, typename Arg>
	bool compare(Arg&& i_arg) const;

private:
    typename std::aligned_storage<ArenaSize,s_actualAlignment>::type m_storage;
	bool m_empty;
};

template<typename T>
using typed_arena = arena<sizeof(embedded_type<T>),alignof(embedded_type<T>)>;

}

#include "ewas_arena.inl"
