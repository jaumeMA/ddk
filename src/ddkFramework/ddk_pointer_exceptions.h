#pragma once

#include <exception>
#include <string>

namespace ddk
{

class unmanaged_pointer_exception : public std::exception
{
public:
	unmanaged_pointer_exception(const std::string& i_reason);

	const char* what() const override;

private:
	const std::string m_reason;
};

class null_pointer_exception : public std::exception
{
public:
	null_pointer_exception(const std::string& i_reason);

	const char* what() const override;

private:
	const std::string m_reason;
};

}