#pragma once

#include "ddk_iterable_result.h"
#include <exception>
#include <string>

namespace ddk
{

class iterable_exception
{
public:
	constexpr iterable_exception(const char* i_reason);

	iterable_error error() const noexcept;

private:
	const char* m_reason;
};

class iteration_exception : public std::exception
{
public:
	iteration_exception(const char* i_reason);
	iteration_exception(const std::string& i_reason);

	const char* what() const noexcept;

private:
	const std::string m_reason;
};

constexpr inline void stop_iteration(const char* i_msg);

}

#include "ddk_iterable_exception.inl"