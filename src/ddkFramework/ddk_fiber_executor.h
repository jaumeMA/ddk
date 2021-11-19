#pragma once

#include "ddk_executor.h"
#include "ddk_fiber.h"
#include "ddk_fiber_sheaf.h"

namespace ddk
{

typedef ddk::polling_executor<ddk::fiber> fiber_polling_executor;
typedef ddk::event_driven_executor<ddk::fiber> fiber_event_driven_executor;
typedef ddk::fire_and_forget_executor<ddk::fiber> fiber_fire_and_forget_executor;

typedef ddk::polling_executor<ddk::fiber_sheaf> fiber_sheaf_polling_executor;
typedef ddk::event_driven_executor<ddk::fiber_sheaf> fiber_sheaf_event_driven_executor;
typedef ddk::fire_and_forget_executor<ddk::fiber_sheaf> fiber_sheaf_fire_and_forget_executor;

}