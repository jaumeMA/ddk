#pragma once

#include <exception>
#include "ddk_fiber_defs.h"

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

struct async_exception : public std::exception
{

};

struct future_exception : public async_exception
{
};


}