#include "ddk_allocator_exceptions.h"

namespace ddk
{

bad_allocation_exception::bad_allocation_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string bad_allocation_exception::reason() const
{
	return m_reason;
}

bad_deallocation_exception::bad_deallocation_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string bad_deallocation_exception::reason() const
{
	return m_reason;
}

}