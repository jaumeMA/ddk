#include "ddk_allocator_exceptions.h"

namespace ddk
{

bad_pointer::bad_pointer(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string bad_pointer::reason() const
{
	return m_reason;
}


bad_alignment_exception::bad_alignment_exception(const std::string& i_reason,void* i_ptr)
: m_reason(i_reason)
, m_ptr(i_ptr)
{
}
std::string bad_alignment_exception::reason() const
{
	return m_reason;
}
void* bad_alignment_exception::get_alloc() const
{
	return m_ptr;
}

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