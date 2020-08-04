#include "ddk_async_exceptions.h"

namespace ddk
{

suspend_exception::suspend_exception(fiber_id i_id)
: m_id(i_id)
{
}
const char* suspend_exception::what() const noexcept
{
	return ("Fiber " + ddk::formatter<std::string>::format(m_id) + "suspended").c_str();
}
fiber_id suspend_exception::get_id() const
{
	return m_id;
}

}