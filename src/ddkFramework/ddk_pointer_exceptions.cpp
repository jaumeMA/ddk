#include "ddk_pointer_exceptions.h"

namespace ddk
{

unmanaged_pointer_exception::unmanaged_pointer_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* unmanaged_pointer_exception::what() const noexcept
{
	return m_reason.c_str();
}

null_pointer_exception::null_pointer_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* null_pointer_exception::what() const noexcept
{
	return m_reason.c_str();
}

}
