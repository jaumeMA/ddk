//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_execution_context.h"

namespace ddk
{
namespace detail
{

struct this_thread_t
{
public:
	this_thread_t();
	void attach_context() const;

private:
	mutable execution_context m_execContext;
};

}

const detail::this_thread_t& this_thread();

}
