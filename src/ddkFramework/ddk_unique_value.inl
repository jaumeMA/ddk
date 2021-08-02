
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
unique_value<T>::unique_value(unique_reference_wrapper<T>&& i_value)
: m_typeInfo(rtti::type_info<T>())
, m_value(std::move(i_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_value<T>::unique_value(unique_reference_wrapper<TT>&& i_value)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(i_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_value<T>::unique_value(unique_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
unique_value<T>& unique_value<T>::operator=(unique_value&& other)
{
	m_typeInfo = std::move(other.m_typeInfo);
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
unique_value<T>& unique_value<T>::operator=(unique_value<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
template<typename TT>
bool unique_value<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
unique_value<T>::operator bool() const
{
	return m_typeInfo.empty() == false;
}
template<typename T>
const rtti::TypeInfo& unique_value<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename unique_value<T>::pointer unique_value<T>::operator->()
{
	return m_value.get();
}
template<typename T>
typename unique_value<T>::const_pointer unique_value<T>::operator->() const
{
	return m_value.get();
}
template<typename T>
typename unique_value<T>::reference unique_value<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename unique_value<T>::const_reference unique_value<T>::operator*() const
{
	return *m_value;
}
template<typename T>
template<typename Interface,typename Visitor>
bool unique_value<T>::may_visit() const
{
	return __may_visit(static_cast<const Interface&>(*m_value),reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename T>
template<typename Interface,typename Visitor>
void unique_value<T>::visit(Visitor&& i_visitor)
{
	__visit(m_typeInfo,static_cast<Interface&>(*m_value),i_visitor);
}
template<typename T>
template<typename Interface,typename Visitor>
void unique_value<T>::visit(Visitor&& i_visitor) const
{
	__visit(m_typeInfo,static_cast<const Interface&>(*m_value),i_visitor);
}

}