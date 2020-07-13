#include "ddk_fiber_scheduler_interface.h"

namespace ddk
{
namespace detail
{

suspend_exception::suspend_exception(fiber_id i_id)
: m_id(i_id)
{
}
const char* suspend_exception::what() const noexcept
{
	return ("Fiber " + ewas::formatter<std::string>::format(m_id) + "suspended").c_str();
}
fiber_id suspend_exception::get_id() const
{
	return m_id;
}

}
}
