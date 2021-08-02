
#include "ddk_rtti.h"

namespace ddk
{

template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>::distributed_object(const distributed_object<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>::distributed_object(distributed_object<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>::distributed_object(const distributed_value<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>::distributed_object(distributed_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
distributed_object<T>& distributed_object<T>::operator=(const distributed_object& other)
{
	m_typeInfo = other.m_typeInfo;
	m_value = other.m_value;

	return *this;
}
template<typename T>
distributed_object<T>& distributed_object<T>::operator=(distributed_object&& other)
{
	m_typeInfo = std::move(other.m_typeInfo);
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>& distributed_object<T>::operator=(const distributed_object<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = other.m_value;

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>& distributed_object<T>::operator=(distributed_object<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>& distributed_object<T>::operator=(const distributed_value<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = other.m_value;

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_object<T>& distributed_object<T>::operator=(distributed_value<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
distributed_object<T>& distributed_object<T>::operator=(std::nullptr_t)
{
	m_typeInfo = rtti::TypeInfo();
	m_value = nullptr;

	return *this;
}
template<typename T>
template<typename TT>
bool distributed_object<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
distributed_object<T>::operator bool() const
{
	return m_typeInfo.empty() == false;
}
template<typename T>
const rtti::TypeInfo& distributed_object<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename distributed_object<T>::pointer distributed_object<T>::operator->()
{
	return m_value.get();
}
template<typename T>
typename distributed_object<T>::const_pointer distributed_object<T>::operator->() const
{
	return m_value.get();
}
template<typename T>
typename distributed_object<T>::reference distributed_object<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename distributed_object<T>::const_reference distributed_object<T>::operator*() const
{
	return *m_value;
}
template<typename T>
template<typename Interface,typename Visitor>
bool distributed_object<T>::may_visit() const
{
	return __may_visit(static_cast<const Interface&>(*m_value),reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename T>
template<typename Interface,typename Visitor>
void distributed_object<T>::visit(Visitor&& i_visitor)
{
	__visit(m_typeInfo,static_cast<Interface&>(*m_value),i_visitor);
}
template<typename T>
template<typename Interface,typename Visitor>
void distributed_object<T>::visit(Visitor&& i_visitor) const
{
	__visit(m_typeInfo,static_cast<const Interface&>(*m_value),i_visitor);
}

}