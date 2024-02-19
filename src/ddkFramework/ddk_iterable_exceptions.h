#pragma once

#include "ddk_iterable_result.h"
#include <exception>
#include <string>

namespace ddk
{

class iterable_exception
{
public:
	enum Type
	{
		Terminated,
		Aborted
	};

	constexpr iterable_exception(Type i_type, const char* i_reason = nullptr);

	Type reason() const noexcept;
	iterable_error error() const noexcept;

private:
	const Type m_type;
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

constexpr inline void terminate_iteration();
constexpr inline void abort_iteration(const char* i_msg);

namespace detail
{

constexpr inline void ___terminate_iteration(const char*);

}
}

#include "ddk_iterable_exception.inl"