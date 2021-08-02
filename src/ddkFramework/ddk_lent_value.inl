
namespace ddk
{

template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_value<T>::lent_value(lent_reference_wrapper<TT> i_value,const rtti::TypeInfo& i_info)
: m_typeInfo(i_info)
, m_value(i_value)
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_value<T>::lent_value(const lent_value<TT>& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_value<T>::lent_value(lent_value<TT>&& other)
: m_typeInfo(rtti::type_info<TT,T>())
, m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_value<T>& lent_value<T>::operator=(const lent_value<TT>& other)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = other.m_value;

	return *this;
}
template<typename T>
TEMPLATE(typename TT)
REQUIRED(IS_BASE_OF(T,TT))
lent_value<T>& lent_value<T>::operator=(lent_value<TT>&&)
{
	m_typeInfo = rtti::type_info<TT,T>();
	m_value = std::move(other.m_value);

	return *this;
}
template<typename T>
template<typename TT>
bool lent_value<T>::is() const
{
	static_assert(std::is_base_of<T,TT>::value,"You shall provide an inherited type from T");

	return m_typeInfo == rtti::type_info<TT,T>();
}
template<typename T>
const rtti::TypeInfo& lent_value<T>::get_type_info() const
{
	return m_typeInfo;
}
template<typename T>
typename lent_value<T>::pointer lent_value<T>::operator->()
{
	return get_raw_ptr(m_value);
}
template<typename T>
typename lent_value<T>::const_pointer lent_value<T>::operator->() const
{
	return get_raw_ptr(m_value);
}
template<typename T>
typename lent_value<T>::reference lent_value<T>::operator*()
{
	return *m_value;
}
template<typename T>
typename lent_value<T>::const_reference lent_value<T>::operator*() const
{
	return *m_value;
}
template<typename T>
template<typename Interface,typename Visitor>
bool lent_value<T>::may_visit() const
{
	return __may_visit(static_cast<const Interface&>(*m_value),reinterpret_cast<const Visitor*>(0xDEAD));
}
template<typename T>
template<typename Interface,typename Visitor>
void lent_value<T>::visit(Visitor&& i_visitor)
{
	typedef mpl::static_if<std::is_const<T>::value,std::add_const<Interface>::type,Interface>::type type_interface;

	__visit(m_typeInfo,static_cast<type_interface&>(*m_value),i_visitor);
}

}