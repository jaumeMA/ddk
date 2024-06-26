//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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
	TEMPLATE(typename TT,typename TTT)
	REQUIRED(IS_LENDABLE_REF(TTT))
	friend lent_value<TT> make_lent_value(TTT&& i_value);
	TEMPLATE(typename TT,typename TTT)
	REQUIRED(IS_LENDABLE_REF(TTT))
	friend lent_value<TT> make_lent_value(TTT&& i_value, const rtti::TypeInfo& i_typeInfo);
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
