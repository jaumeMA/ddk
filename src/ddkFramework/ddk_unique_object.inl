
#include "ddk_rtti.h"

namespace ddk
{

template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_object<T>::unique_object(unique_object<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_object<T>::unique_object(unique_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_object<T>& unique_object<T>::operator=(unique_object<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_object<T>& unique_object<T>::operator=(unique_value<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
unique_object<T>& unique_object<T>::operator=(std::nullptr_t)
{
	m_typeInfo = rtti::TypeInfo();
	m_value = nullptr;

	return *this;
}
template<typename T>
template<typename TT>
bool unique_object<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
unique_object<T>::operator bool() const
{
	return m_typeInfo.empty() == false;
}
template<typename T>
const rtti::TypeInfo& unique_object<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename unique_object<T>::pointer unique_object<T>::operator->()
{
	return m_value.get();
}
template<typename T>
typename unique_object<T>::const_pointer unique_object<T>::operator->() const
{
	return m_value.get();
}
template<typename T>
typename unique_object<T>::reference unique_object<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename unique_object<T>::const_reference unique_object<T>::operator*() const
{
	return *m_value;
}

}