#include "ddk_function_exceptions.h"

namespace ddk
{

call_function_exception::call_function_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string call_function_exception::reason() const
{
    return m_reason;
}

}
