#pragma once

#include "ddk_fiber_defs.h"
#include <exception>
#include <string>

namespace ddk
{

struct suspend_exception : public std::exception
{
	static const int s_valid_code = 0xFFFF;

public:
	suspend_exception(fiber_id i_id);
	suspend_exception(fiber_id i_id, int i_code, const std::string& i_reason);

	fiber_id get_id() const;
	int get_code() const;
	const char* what() const noexcept;
	operator bool() const;

private:
	fiber_id m_id;
	int m_code = s_valid_code;
	std::string m_reason;
};

struct cancel_exception : public std::exception
{
};

SCOPED_ENUM_DECL(AsyncExceptionCode,
				 None,
				 Suspended,
				 Cancelled);

struct async_error
{
public:
	async_error(AsyncExceptionCode i_code, const std::string& i_reason);

	const std::string& what() const;
	AsyncExceptionCode get_code() const;

private:
	AsyncExceptionCode m_code;
	std::string m_reason;
};

struct async_exception : public std::exception
{
public:
    async_exception(const std::string& i_reason, AsyncExceptionCode i_code = AsyncExceptionCode::None, bool i_acquired = false);

    const char* what() const noexcept;
	AsyncExceptionCode get_code() const;
	bool acquired() const;
	async_error as_error() const;

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
