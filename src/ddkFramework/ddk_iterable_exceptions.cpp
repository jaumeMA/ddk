#include "ddk_iterable_exceptions.h"

namespace ddk
{

iterable_operation_forbidden_exception::iterable_operation_forbidden_exception(const char* i_reason)
: m_reason(i_reason)
{
}
const char* iterable_operation_forbidden_exception::what() const noexcept
{
    return m_reason.c_str();
}

}
