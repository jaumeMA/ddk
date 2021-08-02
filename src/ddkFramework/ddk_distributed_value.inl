
#include "ddk_container_exceptions.h"
#include "ddk_hash.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_rtti.h"
#include "ddk_reference_wrapper.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_system_reference_wrapper_allocator.h"

namespace ddk
{

template<typename T>
distributed_value<T>::distributed_value(const distributed_reference_wrapper<T>& i_value)
: m_typeInfo(rtti::type_info<T>())
, m_value(i_value)
{
}
template<typename T>
distributed_value<T>::distributed_value(const distributed_reference_wrapper<T>& other,const rtti::TypeInfo& i_typeInfo)
: m_typeInfo(i_typeInfo)
, m_value(i_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_value<T>::distributed_value(const distributed_value<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_value<T>::distributed_value(distributed_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
distributed_value<T>& distributed_value<T>::operator=(const distributed_value& other)
{
	m_typeInfo = other.m_typeInfo;
	m_value = other.m_value;

	return *this;
}
template<typename T>
distributed_value<T>& distributed_value<T>::operator=(distributed_value&& other)
{
	m_typeInfo = std::move(other.m_typeInfo);
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_value<T>& distributed_value<T>::operator=(const distributed_value<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = other.m_value;

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
distributed_value<T>& distributed_value<T>::operator=(distributed_value<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
template<typename TT>
bool distributed_value<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
const rtti::TypeInfo& distributed_value<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename distributed_value<T>::pointer distributed_value<T>::operator->()
{
	return m_value.get();
}
template<typename T>
typename distributed_value<T>::const_pointer distributed_value<T>::operator->() const
{
	return m_value.get();
}
template<typename T>
typename distributed_value<T>::reference distributed_value<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename distributed_value<T>::const_reference distributed_value<T>::operator*() const
{
	return *m_value;
}

}