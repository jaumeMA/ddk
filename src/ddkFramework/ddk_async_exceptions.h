#pragma once

#include <exception>
#include "ddk_fiber_defs.h"
#include <string>

namespace ddk
{

struct suspend_exception : public std::exception
{
public:
	suspend_exception(fiber_id i_id);

	const char* what() const noexcept;
	fiber_id get_id() const;

private:
	fiber_id m_id;
};

SCOPED_ENUM_DECL(AsyncExceptionCode,
				 None);

struct async_exception : public std::exception
{
public:
    async_exception(const std::string& i_reason, AsyncExceptionCode i_code = AsyncExceptionCode::None, bool i_acquired = false);

    const char* what() const noexcept;
	AsyncExceptionCode get_code() const;
	bool acquired() const;

private:
    std::string m_reason;
	AsyncExceptionCode m_code;
	bool m_acquired;
};

struct future_exception : public async_exception
{
public:
    using async_exception::async_exception;
};


}
