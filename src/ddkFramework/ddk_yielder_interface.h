//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_yielder_context.h"

namespace ddk
{
namespace detail
{

class yielder_interface
{
protected:
	virtual ~yielder_interface() = default;

public:
	virtual void yield(yielder_context*) = 0;
	virtual bool suspend() = 0;
};

}
}
