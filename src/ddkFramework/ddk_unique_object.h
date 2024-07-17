//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_unique_pointer_wrapper.h"
#include "ddk_rtti_defs.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
class unique_object
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class unique_object;
	template<typename TT>
	friend lent_object<TT> lend(const unique_object<TT>&);
	template<typename TT>
	friend distributed_value<TT> promote_to_value(const unique_object<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	unique_object() = default;
	unique_object(const unique_object& other) = delete;
	unique_object(unique_object&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_object(unique_value<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_object(unique_object<TT>&& other);
	unique_object& operator=(const unique_object& other) = delete;
	unique_object& operator=(unique_object&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_object& operator=(unique_object<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_object& operator=(unique_value<TT>&& other);
	unique_object& operator=(std::nullptr_t);
	template<typename TT>
	inline bool is() const;
	inline operator bool() const;
	inline const rtti::TypeInfo& get_type_info() const;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline reference operator*();
	inline const_reference operator*() const;

private:
	rtti::TypeInfo m_typeInfo;
	unique_pointer_wrapper<T> m_value;
};

}

#include "ddk_unique_object.inl"