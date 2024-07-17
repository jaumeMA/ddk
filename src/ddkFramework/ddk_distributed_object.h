//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_rtti_defs.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
class distributed_object
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class distributed_object;
	template<typename TT>
	friend lent_object<TT> lend(const distributed_object<TT>&);
	template<typename TT>
	friend distributed_value<TT> promote_to_value(const distributed_object<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	distributed_object() = default;
	distributed_object(const distributed_object& other) = default;
	distributed_object(distributed_object&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object(const distributed_object<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object(const distributed_value<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object(distributed_value<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object(distributed_object<TT>&& other);
	distributed_object& operator=(const distributed_object& other);
	distributed_object& operator=(distributed_object&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object& operator=(const distributed_object<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object& operator=(distributed_object<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object& operator=(const distributed_value<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_object& operator=(distributed_value<TT>&& other);
	distributed_object& operator=(std::nullptr_t);
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
	distributed_pointer_wrapper<T> m_value;
};

}

#include "ddk_distributed_object.inl"