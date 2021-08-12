#pragma once

#include "ddk_unique_reference_wrapper.h"
#include "ddk_rtti_defs.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename>
class lent_value;

template<typename T>
class unique_value
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class unique_object;
	template<typename>
	friend class unique_value;
	template<typename TT>
	friend lent_value<TT> lend(const unique_value<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	unique_value() = delete;
	unique_value(const unique_value& other) = delete;
	unique_value(unique_value&& other) = default;
	unique_value(unique_reference_wrapper<T>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_value(unique_reference_wrapper<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_value(unique_value<TT>&& other);
	unique_value& operator=(const unique_value& other) = delete;
	unique_value& operator=(unique_value&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	unique_value& operator=(unique_value<TT>&& other);
	template<typename TT>
	inline bool is() const;
	inline const rtti::TypeInfo& get_type_info() const;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline reference operator*();
	inline const_reference operator*() const;

private:
	rtti::TypeInfo m_typeInfo;
	unique_reference_wrapper<T> m_value;
};

}

#include "ddk_unique_value.inl"