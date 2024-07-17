#include "ddk_iterable_exceptions.h"

namespace ddk
{

iterable_exception::Type iterable_exception::reason() const noexcept
{
	return m_type;
}
iterable_error iterable_exception::error() const noexcept
{
	return { IterableError::UserError,m_reason };
}

iteration_exception::iteration_exception(const char* i_reason)
: m_reason(i_reason)
{
}
iteration_exception::iteration_exception(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* iteration_exception::what() const noexcept
{
	return m_reason.c_str();
}

}
