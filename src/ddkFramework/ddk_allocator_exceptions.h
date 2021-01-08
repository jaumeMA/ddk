#pragma once

#include <exception>
#include <string>

namespace ddk
{

class bad_allocation_exception : public std::exception
{
public:
	bad_allocation_exception(const std::string& i_reason);

	std::string reason() const;

private:
	const std::string m_reason;
};

class bad_deallocation_exception : public std::exception
{
public:
	bad_deallocation_exception(const std::string& i_reason);

	std::string reason() const;

private:
	const std::string m_reason;
};

}
