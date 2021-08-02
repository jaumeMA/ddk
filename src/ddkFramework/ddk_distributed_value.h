#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_rtti_defs.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename T>
class distributed_value
{
	static_assert(std::is_polymorphic<T>::value,"You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename>
	friend class distributed_object;
	template<typename>
	friend class distributed_value;
	template<typename TT,typename ... Args>
	friend distributed_value<TT> make_distributed_value(Args&& ...);
	template<typename TT>
	friend lent_value<TT> lend(const distributed_value<TT>&);
	template<typename TT>
	friend distributed_value<TT> promote_to_value(const distributed_object<TT>&);

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	distributed_value() = delete;
	distributed_value(const distributed_value& other) = default;
	distributed_value(distributed_value&& other) = default;
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_value(const distributed_value<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_value(distributed_value<TT>&& other);
	distributed_value& operator=(const distributed_value& other);
	distributed_value& operator=(distributed_value&& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_value& operator=(const distributed_value<TT>& other);
	TEMPLATE(typename TT)
	REQUIRES(IS_BASE_OF(T,TT))
	distributed_value& operator=(distributed_value<TT>&& other);
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
	void visit(Visitor&& i_visitor) const;
	template<typename Interface,typename Visitor>
	void visit(Visitor&& i_visitor);

private:
	distributed_value(const distributed_reference_wrapper<T>& i_value);
	distributed_value(const distributed_reference_wrapper<T>& other, const rtti::TypeInfo& i_typeInfo);

	rtti::TypeInfo m_typeInfo;
	distributed_reference_wrapper<T> m_value;
};

}

#include "ddk_distributed_value.inl"