#pragma once

#include "ddk_lent_object.h"
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
class distributed_value;
template<typename>
class unique_value;

template<typename T>
class lent_value
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class lent_object;
	template<typename>
	friend class lent_value;
	template<typename TT>
	friend lent_value<TT> lend(const distributed_value<TT>&);
	template<typename TT>
	friend lent_value<TT> lend(const unique_value<TT>&);
	template<typename TT>
	friend lent_value<TT> promote_to_value(const lent_object<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	lent_value() = delete;
	lent_value(const lent_value&) = default;
	lent_value(lent_value&&) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_value(const lent_value<TT>&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_value(lent_value<TT>&&);
	lent_value& operator=(const lent_value& other) = default;
	lent_value& operator=(lent_value&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_value& operator=(const lent_value<TT>&);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	lent_value& operator=(lent_value<TT>&&);
	template<typename TT>
	inline bool is() const;
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
	lent_value(lent_reference_wrapper<TT> other, const rtti::TypeInfo& i_info);

	rtti::TypeInfo m_typeInfo;
	lent_reference_wrapper<T> m_value;
};

}

#include "ddk_lent_value.inl"
