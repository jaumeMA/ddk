//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_connection_base.h"

namespace ddk
{
namespace detail
{

class signal_connector
{
public:
	virtual bool disconnect(const connection_base&) const = 0;
};

}
}