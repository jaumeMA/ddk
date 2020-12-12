#include "ddk_reference_exception.h"

namespace ddk
{

bad_ptr_exception::bad_ptr_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string bad_ptr_exception::reason() const
{
	return m_reason;
}

}