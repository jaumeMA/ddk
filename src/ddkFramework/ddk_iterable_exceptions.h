#pragma once

#include <exception>
#include <string>

namespace ddk
{

class iterable_operation_forbidden : public std::exception
{
public:
    iterable_operation_forbidden(const char* i_reason);

    const char* what() const noexcept;

private:
    std::string m_reason;
};

}
