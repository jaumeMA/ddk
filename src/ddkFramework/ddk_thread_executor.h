//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_executor.h"
#include "ddk_execution_model.h"
#include "ddk_thread.h"
#include "ddk_thread_sheaf.h"

namespace ddk
{

typedef ddk::executor<ddk::thread,detail::polling_execution_model> thread_polling_executor;
typedef ddk::executor<ddk::thread,detail::async_execution_model> thread_event_driven_executor;
typedef ddk::executor<ddk::thread,detail::forward_execution_model> thread_fire_and_forget_executor;

typedef ddk::executor<ddk::thread_sheaf,detail::polling_execution_model> thread_sheaf_polling_executor;
typedef ddk::executor<ddk::thread_sheaf,detail::async_execution_model> thread_sheaf_event_driven_executor;
typedef ddk::executor<ddk::thread_sheaf,detail::forward_execution_model> thread_sheaf_fire_and_forget_executor;

}
