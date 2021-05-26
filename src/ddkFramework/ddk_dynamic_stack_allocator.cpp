#include "ddk_dynamic_stack_allocator.h"
#include "ddk_macros.h"

namespace ddk
{

stack_alloc_dist_ref make_default_dynamic_stack_allocator()
{
	return make_distributed_reference<detail::default_dynamic_stack_allocator>();
}

}