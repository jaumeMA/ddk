#include "ddk_iterable_exceptions.h"

namespace ddk
{

iterable_operation_forbidden::iterable_operation_forbidden(const char* i_reason)
: m_reason(i_reason)
{
}
const char* iterable_operation_forbidden::what() const noexcept
{
    return m_reason.c_str();
}

}
