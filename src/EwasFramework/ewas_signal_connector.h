#pragma once

#include "ewas_connection_base.h"

namespace ewas
{
namespace detail
{

class signal_connector
{
public:
	virtual void disconnect(const connection_base&) const = 0;
};

}
}