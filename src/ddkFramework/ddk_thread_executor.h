#pragma once

#include "ddk_executor.h"
#include "ddk_thread_executor_capabilities.h"

namespace ddk
{

typedef ddk::polling_executor<ddk::thread> thread_polling_executor;
typedef ddk::event_driven_executor<ddk::thread> thread_event_driven_executor;
typedef ddk::fire_and_forget_executor<ddk::thread> thread_fire_and_forget_executor;

typedef ddk::polling_executor<ddk::thread_sheaf> thread_sheaf_polling_executor;
typedef ddk::event_driven_executor<ddk::thread_sheaf> thread_sheaf_event_driven_executor;
typedef ddk::fire_and_forget_executor<ddk::thread_sheaf> thread_sheaf_fire_and_forget_executor;

}
