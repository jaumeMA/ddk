#pragma once

#include "ddk_dynamic_visitor.h"
#include "ddk_system_allocator.h"
#include "ddk_rtti_defs.h"
#include "ddk_any_value.h"
#include "ddk_smart_pointer_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename>
class distributed_object;
template<typename>
class unique_object;

template<typename T>
class lent_object
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class lent_object;
	template<typename TT>
	friend lent_object<TT> lend(const distributed_object<TT>&);
	template<typename TT>
	friend lent_object<TT> lend(const unique_object<TT>&);
	template<typename TT>
	friend lent_value<TT> promote_to_value(const lent_object<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	lent_object() = default;
	lent_object(const lent_object&) = default;
	lent_object(lent_object&&) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object(const lent_object<TT>&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object(lent_object<TT>&&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object(const lent_value<TT>&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object(lent_value<TT>&&);
	lent_object& operator=(const lent_object& other) = default;
	lent_object& operator=(lent_object&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object& operator=(const lent_object<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object& operator=(lent_object<TT>&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object& operator=(const lent_value<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object& operator=(lent_value<TT>&& other);
	template<typename TT>
	inline bool is() const;
	inline operator bool() const;
	inline const rtti::TypeInfo& get_type_info() const;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline reference operator*();
	inline const_reference operator*() const;
	template<typename Interface,typename Visitor>
	inline bool may_visit() const;
	template<typename Interface,typename Visitor>
	void visit(Visitor&& i_visitor);

private:
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_object(lent_pointer_wrapper<TT> other,const rtti::TypeInfo& i_info);

	rtti::TypeInfo m_typeInfo;
	lent_pointer_wrapper<T> m_value;
};

}

#include "ddk_lent_object.inl"