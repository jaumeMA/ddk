//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_context_executor_interface.h"

namespace ddk
{

class thread_executor_interface : public context_executor_interface
{
public:
	virtual bool set_affinity(const cpu_set_t& i_set) = 0;
};

typedef unique_reference_wrapper<thread_executor_interface> thread_executor_unique_ref;
typedef unique_reference_wrapper<const thread_executor_interface> thread_executor_const_unique_ref;
typedef unique_pointer_wrapper<thread_executor_interface> thread_executor_unique_ptr;
typedef unique_pointer_wrapper<const thread_executor_interface> thread_executor_const_unique_ptr;

typedef lent_reference_wrapper<thread_executor_interface> thread_executor_lent_ref;
typedef lent_reference_wrapper<const thread_executor_interface> thread_executor_const_lent_ref;
typedef lent_pointer_wrapper<thread_executor_interface> thread_executor_lent_ptr;
typedef lent_pointer_wrapper<const thread_executor_interface> thread_executor_const_lent_ptr;

}