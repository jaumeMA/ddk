#pragma once

#include <exception>

namespace ddk
{

struct bad_allocation_exception : public std::exception
{
};

}
