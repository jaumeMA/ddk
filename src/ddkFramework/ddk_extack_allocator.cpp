#include "ddk_extack_allocator.h"
#include "ddk_allocator_exceptions.h"

#if defined(_WIN32)
#include <malloc.h>
#else
#include <alloca.h>
#endif

namespace ddk
{

}