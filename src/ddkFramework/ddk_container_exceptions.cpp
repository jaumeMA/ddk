#include "ddk_container_exceptions.h"

namespace ddk
{

bad_access_exception::bad_access_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
std::string bad_access_exception::reason() const
{
    return m_reason;
}

}
