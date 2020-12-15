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

async_error::async_error(AsyncExceptionCode i_code,const std::string& i_reason)
: m_code(i_code)
, m_reason(i_reason)
{
}
const std::string& async_error::what() const
{
	return m_reason;
}
AsyncExceptionCode async_error::get_code() const
{
	return m_code;	
}

async_exception::async_exception(const std::string& i_reason, AsyncExceptionCode i_code, bool i_acquired)
: m_reason(i_reason)
, m_code(i_code)
, m_acquired(i_acquired)
{
}
const char* async_exception::what() const noexcept
{
    return m_reason.c_str();
}
AsyncExceptionCode async_exception::get_code() const
{
	return m_code;
}
bool async_exception::acquired() const
{
	return m_acquired;
}
async_error async_exception::as_error() const
{
	return {m_code, m_reason};
}

}
