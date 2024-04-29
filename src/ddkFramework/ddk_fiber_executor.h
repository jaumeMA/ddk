//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_executor.h"
#include "ddk_fiber.h"
#include "ddk_fiber_sheaf.h"

namespace ddk
{

typedef ddk::executor<ddk::fiber,detail::polling_execution_model> fiber_polling_executor;
typedef ddk::executor<ddk::fiber,detail::async_execution_model> fiber_event_driven_executor;
typedef ddk::executor<ddk::fiber,detail::forward_execution_model> fiber_fire_and_forget_executor;

typedef ddk::executor<ddk::fiber_sheaf,detail::polling_execution_model> fiber_sheaf_polling_executor;
typedef ddk::executor<ddk::fiber_sheaf,detail::async_execution_model> fiber_sheaf_event_driven_executor;
typedef ddk::executor<ddk::fiber_sheaf,detail::forward_execution_model> fiber_sheaf_fire_and_forget_executor;

}