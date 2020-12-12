
#include "ddk_container_exceptions.h"
#include "ddk_hash.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_rtti.h"
#include "ddk_reference_wrapper.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_system_reference_wrapper_allocator.h"

namespace ddk
{

template<typename TT,typename ... Args>
inherited_value<TT> make_inherited_value(Args&& ... i_args)
{
	return inherited_value<TT>(std::forward<Args>(i_args) ...);
}


template<typename T,typename Allocator>
template<typename TT>
inherited_value<T,Allocator>::inherited_value(const inherited_value<TT,Allocator>& other)
: m_typeInfo(other.m_typeInfo)
, m_value(other.m_value)
{
}
template<typename T,typename Allocator>
template<typename TT>
inherited_value<T,Allocator>::inherited_value(inherited_value<TT,Allocator>&& other)
: m_typeInfo(std::move(other.m_typeInfo))
, m_value(std::move(other.m_value))
{
}
template<typename T, typename Allocator>
template<typename ... Args>
inherited_value<T,Allocator>::inherited_value(Args&& ... i_args)
: m_typeInfo(rtti<T>())
{
	if(void* mem = m_allocator.allocate(1,sizeof(T)))
	{
		T* newValue = new(mem) T(std::forward<Args>(i_args) ...);

		m_value = as_shared_reference(newValue,get_reference_wrapper_deleter<T>(m_allocator));
	}
	else
	{
		throw bad_allocation_exception{ "Could not allocate inherited value" };
	}
}
template<typename T,typename Allocator>
inherited_value<T,Allocator>& inherited_value<T,Allocator>::operator=(const inherited_value& other)
{
	m_typeInfo = other.m_typeInfo;
	m_value = other.m_value;

	return *this;
}
template<typename T,typename Allocator>
inherited_value<T,Allocator>& inherited_value<T,Allocator>::operator=(inherited_value&& other)
{
	m_typeInfo = std::move(other.m_typeInfo);
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T,typename Allocator>
template<typename TT>
inherited_value<T,Allocator>& inherited_value<T,Allocator>::operator=(const inherited_value<TT,Allocator>& other)
{
	m_typeInfo = other.m_typeInfo;
	m_value = other.m_value;

	return *this;
}
template<typename T,typename Allocator>
template<typename TT>
inherited_value<T,Allocator>& inherited_value<T,Allocator>::operator=(inherited_value<TT,Allocator>&& other)
{
	m_typeInfo = std::move(other.m_typeInfo);
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T, typename Allocator>
template<typename TT>
bool inherited_value<T,Allocator>::is() const
{
    static_assert(std::is_base_of<T,TT>::value, "You shall provide an inherited type from T");

    return m_typeInfo == rtti<TT>();
}
template<typename T, typename Allocator>
inherited_value<T,Allocator>::operator bool() const
{
    return m_typeInfo.empty() == false;
}
template<typename T, typename Allocator>
const TypeInfo& inherited_value<T,Allocator>::get_type_info() const
{
    return m_typeInfo;
}
template<typename T, typename Allocator>
typename inherited_value<T,Allocator>::pointer inherited_value<T,Allocator>::operator->()
{
	return m_value.get();
}
template<typename T, typename Allocator>
typename inherited_value<T,Allocator>::const_pointer inherited_value<T,Allocator>::operator->() const
{
	return m_value.get();
}
template<typename T, typename Allocator>
typename inherited_value<T,Allocator>::reference inherited_value<T,Allocator>::operator*()
{
	return *m_value;
}
template<typename T, typename Allocator>
typename inherited_value<T,Allocator>::const_reference inherited_value<T,Allocator>::operator*() const
{
	return *m_value;
}
template<typename T,typename Allocator>
template<typename Visitor>
bool inherited_value<T,Allocator>::may_visit() const
{
	return __may_visit(*m_value,reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename T,typename Allocator>
template<typename Visitor>
any_value inherited_value<T,Allocator>::visit(Visitor&& i_visitor) const
{
	return __visit(m_typeInfo,*m_value,i_visitor);
}

}
