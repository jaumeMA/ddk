//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <utility>
#include <type_traits>

namespace ddk
{

//internal type
template<typename T>
struct embedded_type;

template<typename T>
struct embedded_type<T&>
{
public:
	//avoid right references
	typedef typename std::remove_cv<T>::type raw_type;
	typedef T internal_type;
	typedef T& ref_type;
	typedef T& cref_type;
	typedef T& rref_type;
	typedef T* pointer_type;
	typedef typename std::add_const<T>::type* cpointer_type;

	constexpr embedded_type(internal_type& other);
	constexpr embedded_type(const embedded_type<T&>& other);
	embedded_type() = delete;
	inline T& operator=(internal_type& other) = delete;
	inline T& operator=(internal_type&& other) = delete;
	inline T& operator=(embedded_type<T&>&& other) = delete;
	template<typename Type>
	inline embedded_type& operator=(Type&& val);
	constexpr bool operator==(const embedded_type<T&>& other) const;
	constexpr inline rref_type extract() &&;
	constexpr cref_type get() const;
	constexpr ref_type get();
	constexpr pointer_type get_ptr();
	constexpr cref_type operator*() const;
	constexpr ref_type operator*();
	constexpr pointer_type operator->();
	constexpr cpointer_type operator->() const;
	constexpr operator ref_type();
	constexpr operator cref_type() const;
	inline ref_type inplace_construct(T& val);
	inline ref_type inplace_assign(T& val);
	constexpr inline void inplace_destroy();
	inline static bool construct(void* address,T& val);
	inline static bool assign(void* address,T& val);
	constexpr inline static bool destroy(void* address);

private:
	internal_type& m_data;
};

template<typename T>
struct embedded_type<T&&>
{
public:
	//avoid right references
	typedef typename std::remove_cv<T>::type raw_type;
	typedef T internal_type;
	typedef T&& ref_type;
	typedef T&& cref_type;
	typedef T&& rref_type;
	typedef T* pointer_type;
	typedef typename std::add_const<T>::type* cpointer_type;

	constexpr embedded_type(T&& other);
	constexpr embedded_type(embedded_type&& other);
	//this is a ref, we do not allow assignments beyond construction
	embedded_type() = delete;
	embedded_type(internal_type& other) = delete;
	T& operator=(internal_type& other) = delete;
	T& operator=(internal_type&& other) = delete;
	T& operator=(embedded_type<const T&>&& other) = delete;
	constexpr bool operator==(const embedded_type<T&&>& other) const;
	constexpr cref_type get() const;
	constexpr ref_type get();
	constexpr pointer_type get_ptr();
	constexpr inline rref_type extract() &&;
	constexpr cref_type operator*() const;
	constexpr ref_type operator*();
	constexpr pointer_type operator->();
	constexpr cpointer_type operator->() const;
	constexpr operator ref_type();
	constexpr operator cref_type() const;
	inline ref_type inplace_construct(T&& val);
	inline ref_type inplace_assign(T&& val);
	constexpr inline void inplace_destroy();
	inline static bool construct(void* address,T&& val);
	inline static bool assign(void* address,T&& val);
	constexpr inline static bool destroy(void* address);
	inline static bool swap(void* addressA,internal_type&& valA,void* addressB,internal_type&& valB);

private:
	internal_type&& m_data;
};

template<typename T>
struct embedded_type
{
public:
	//avoid deleted methods
	typedef typename std::remove_cv<T>::type raw_type;
	typedef T internal_type;
	typedef T& ref_type;
	typedef typename std::add_const<T>::type& cref_type;
	typedef T&& rref_type;
	typedef T* pointer_type;
	typedef typename std::add_const<T>::type* cpointer_type;

	constexpr embedded_type() = default;
	constexpr embedded_type(const embedded_type<T>& other);
	constexpr embedded_type(embedded_type<T>&& other);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args&&...))
	constexpr embedded_type(Args&& ... i_args);
	constexpr inline embedded_type& operator=(const internal_type& other);
	constexpr inline embedded_type& operator=(internal_type&& other);
	constexpr inline embedded_type& operator=(const embedded_type<T>& other);
	constexpr inline embedded_type& operator=(embedded_type<T>&& other);
	constexpr bool operator==(const embedded_type<T>& other) const;
	constexpr cref_type get() const;
	constexpr ref_type get();
	constexpr cpointer_type get_ptr() const;
	constexpr pointer_type get_ptr();
	constexpr inline rref_type extract() &&;
	constexpr cref_type operator*() const;
	constexpr ref_type operator*();
	constexpr pointer_type operator->();
	constexpr cpointer_type operator->() const;
	constexpr inline operator rref_type() &&;
	template<typename ... Args>
	inline T& inplace_construct(Args&& ... i_args);
	template<typename ... Args>
	constexpr T& inplace_assign(Args&& ... i_args);
	constexpr inline void inplace_destroy();
	template<typename ... Args>
	inline static bool construct(void* address,Args&& ... i_args);
	template<typename Type>
	constexpr inline static bool assign(void* address,Type&& val);
	constexpr inline static bool destroy(void* address);

private:
	internal_type  m_data;
};

}

#include "ddk_embedded_type.inl"