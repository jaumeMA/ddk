#pragma once

#include "type_id.h"
#include "ddk_rtti_defs.h"

namespace ddk
{

struct visitable_type_t;
typedef Id<std::pair<TypeInfo,TypeInfo>,visitable_type_t> visitable_type_info;

}