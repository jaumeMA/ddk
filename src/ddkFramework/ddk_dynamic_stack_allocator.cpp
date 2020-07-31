#include "ddk_dynamic_stack_allocator.h"
#include "ddk_macros.h"

namespace ddk
{

stack_alloc_shared_ref make_default_dynamic_stack_allocator()
{
	return make_shared_reference<detail::default_dynamic_stack_allocator>();
}

}