
#include "ddk_exception_handler.h"

namespace ddk
{

constexpr iterable_exception::iterable_exception(Type i_type, const char* i_reason)
: m_type(i_type)
, m_reason(i_reason)
{
}

}