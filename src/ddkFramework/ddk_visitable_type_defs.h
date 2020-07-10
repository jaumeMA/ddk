#pragma once

#include "type_id.h"
#include "ewas_rtti_defs.h"

namespace ewas
{

struct visitable_type_t;
typedef Id<std::pair<TypeInfo,TypeInfo>,visitable_type_t> visitable_type_info;

}