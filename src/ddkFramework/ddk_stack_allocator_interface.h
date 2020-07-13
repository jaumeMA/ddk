#pragma once

#include <utility>
#include "shared_reference_wrapper.h"
#include "reference_wrapper.h"

extern "C"
{
	void set_curr_thread_stack_limit(void*);
}

namespace ddk
{

class stack_allocator_interface
{
public:
	static const size_t k_maxNumStackPages = 60;

	virtual ~stack_allocator_interface() = default;
	virtual void* reserve(size_t) const = 0;
	virtual void* allocate(void*,size_t) const = 0;
	virtual bool reallocate(std::pair<void*,void*>&, void*) const = 0;
	virtual void deallocate(void*,size_t) const = 0;
	virtual void release(void*,size_t) const = 0;

	static size_t s_pageSize;
};

typedef ewas::shared_reference_wrapper<stack_allocator_interface> stack_alloc_shared_ref;
typedef ewas::shared_reference_wrapper<const stack_allocator_interface> stack_alloc_const_shared_ref;
typedef ewas::shared_pointer_wrapper<stack_allocator_interface> stack_alloc_shared_ptr;
typedef ewas::shared_pointer_wrapper<const stack_allocator_interface> stack_alloc_const_shared_ptr;

typedef ewas::lent_reference_wrapper<stack_allocator_interface> stack_alloc_lent_ref;
typedef ewas::lent_reference_wrapper<const stack_allocator_interface> stack_alloc_const_lent_ref;
typedef ewas::lent_pointer_wrapper<stack_allocator_interface> stack_alloc_lent_ptr;
typedef ewas::lent_pointer_wrapper<const stack_allocator_interface> stack_alloc_const_lent_ptr;

template<typename Allocator, typename ... Args>
stack_alloc_shared_ref make_stack_allocator(Args&& ... i_args)
{
	return make_shared_reference<Allocator>(std::forward<Args>(i_args) ...);
}

}


