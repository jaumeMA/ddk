#pragma once

#include "type_id.h"
#include "ewas_scoped_enum.h"

namespace ewas
{

struct sender_id_t;
typedef Id<size_t,sender_id_t> sender_id;

struct receiver_id_t;
typedef Id<size_t,receiver_id_t> receiver_id;

static const unsigned long k_messageQueueUpdateTimeInMS = 500;

}