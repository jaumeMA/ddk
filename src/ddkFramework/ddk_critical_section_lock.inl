
namespace ddk
{

template<typename Interface>
critical_section_lock<Interface>::critical_section_lock(Interface& i_interface, Reentrancy i_reentrancy)
: m_interface(i_interface)
, m_criitcalSection(i_interface.enterCriticalSection<IAccessProvider::WRITING>(i_reentrancy))
{
}
template<typename Interface>
critical_section_lock<Interface>::critical_section_lock(critical_section_lock&& other)
: m_interface(other.m_interface)
, m_criitcalSection(std::move(other.m_criitcalSection))
{
}
template<typename Interface>
critical_section_lock<Interface>::~critical_section_lock()
{
	m_interface.leaveCriticalSection<IAccessProvider::WRITING>(std::move(m_criitcalSection));
}
template<typename Interface>
auto critical_section_lock<Interface>::operator->() const
{
	return m_criitcalSection.operator->();
}
template<typename Interface>
auto critical_section_lock<Interface>::operator*() const
{
	return m_criitcalSection.operator*();
}

template<typename Interface>
const_critical_section_lock<Interface>::const_critical_section_lock(const Interface& i_interface,Reentrancy i_reentrancy)
: m_interface(i_interface)
, m_criitcalSection(i_interface.enterCriticalSection<IAccessProvider::READING>(i_reentrancy))
{
}
template<typename Interface>
const_critical_section_lock<Interface>::const_critical_section_lock(const_critical_section_lock&& other)
: m_interface(other.m_interface)
, m_criitcalSection(std::move(other.m_criitcalSection))
{
}
template<typename Interface>
const_critical_section_lock<Interface>::const_critical_section_lock(critical_section_lock<Interface>&& other)
: m_interface(other.m_interface)
, m_criitcalSection(std::move(other.m_criitcalSection))
{
}
template<typename Interface>
const_critical_section_lock<Interface>::~const_critical_section_lock()
{
	m_interface.leaveCriticalSection<IAccessProvider::READING>(std::move(m_criitcalSection));
}
template<typename Interface>
auto const_critical_section_lock<Interface>::operator->() const
{
	return m_criitcalSection.operator->();
}
template<typename Interface>
auto const_critical_section_lock<Interface>::operator*() const
{
	return m_criitcalSection.operator*();
}

}