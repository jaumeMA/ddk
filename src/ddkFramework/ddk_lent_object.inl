
namespace ddk
{

template<typename T>
lent_object<T>::lent_object(const lent_value<T>& other)
: m_typeInfo(other.m_typeInfo)
, m_value(other.m_value)
{
}
template<typename T>
lent_object<T>::lent_object(lent_value<T>&& other)
: m_typeInfo(std::move(other.m_typeInfo))
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>::lent_object(const lent_object<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>::lent_object(lent_object<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>::lent_object(const lent_value<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(other.m_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>::lent_object(lent_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>::lent_object(lent_pointer_wrapper<TT> i_value,const rtti::TypeInfo& i_info)
: m_typeInfo(i_info)
, m_value(i_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>& lent_object<T>::operator=(const lent_object<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = other.m_value;

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>& lent_object<T>::operator=(lent_object<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>& lent_object<T>::operator=(const lent_value<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_object<T>& lent_object<T>::operator=(lent_value<TT>&& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
template<typename TT>
bool lent_object<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
lent_object<T>::operator bool() const
{
	return m_typeInfo.empty() == false;
}
template<typename T>
const rtti::TypeInfo& lent_object<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename lent_object<T>::pointer lent_object<T>::operator->()
{
	return get_raw_ptr(m_value);
}
template<typename T>
typename lent_object<T>::const_pointer lent_object<T>::operator->() const
{
	return get_raw_ptr(m_value);
}
template<typename T>
typename lent_object<T>::reference lent_object<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename lent_object<T>::const_reference lent_object<T>::operator*() const
{
	return *m_value;
}
template<typename T>
template<typename Interface,typename Visitor>
bool lent_object<T>::may_visit() const
{
	return __may_visit(static_cast<const Interface&>(*m_value),reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename T>
template<typename Interface,typename Visitor>
void lent_object<T>::visit(Visitor&& i_visitor)
{
	typedef typename mpl::static_if<std::is_const<T>::value,typename std::add_const<Interface>::type,Interface>::type type_interface;

	__visit(m_typeInfo,static_cast<type_interface&>(*m_value),i_visitor);
}

}
