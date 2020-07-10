#include "critical_section_context.h"
#include "ewas_macros.h"
#include <utility>

critical_section_context::critical_section_context(const detail::context_acquired_t& i_acq)
: m_acquired(i_acq.m_acquired)
{
}
critical_section_context::critical_section_context(critical_section_context&& other)
: m_acquired(false)
{
	std::swap(m_acquired,other.m_acquired);
}
critical_section_context::operator bool() const
{
	return m_acquired;
}
critical_section_context& critical_section_context::operator=(critical_section_context&& other)
{
	m_acquired = other.m_acquired;
	other.m_acquired = false;

	return *this;
}