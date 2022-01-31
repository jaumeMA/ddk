#pragma once

#include "ddk_critical_section.h"

namespace ddk
{

template<typename Interface>
struct const_critical_section_lock
{
	typedef typename Interface::traits_t traits_t;

public:
	const_critical_section_lock(const Interface& i_interface, Reentrancy i_reentrancy);
	const_critical_section_lock(const const_critical_section_lock&) = delete;
	const_critical_section_lock(const_critical_section_lock&&) = delete;
	~const_critical_section_lock();

	const_critical_section_lock& operator=(const const_critical_section_lock&) = delete;
	const_critical_section_lock& operator=(const_critical_section_lock&&) = delete;

	auto operator->() const;
	auto operator*() const;

private:
	const Interface& m_interface;
	const_critical_section<traits_t> m_criitcalSection;
};
template<typename Interface>
const_critical_section_lock(const Interface&, Reentrancy) -> const_critical_section_lock<Interface>;

template<typename Interface>
struct critical_section_lock
{
	typedef typename Interface::traits_t traits_t;

public:
	critical_section_lock(Interface& i_interface, Reentrancy i_reentrancy);
	critical_section_lock(const critical_section_lock&) = delete;
	critical_section_lock(critical_section_lock&&) = delete;
	~critical_section_lock();

	auto operator->() const;
	auto operator*() const;

private:
	Interface& m_interface;
	critical_section<traits_t> m_criitcalSection;
};
template<typename Interface>
critical_section_lock(Interface&) -> critical_section_lock<Interface>;

}

#include "ddk_critical_section_lock.inl"