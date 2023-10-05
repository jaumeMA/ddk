#pragma once

#include "ddk_executor_interface.h"

namespace ddk
{

class context_executor_interface : public executor_interface<void()>
{
public:
    using typename executor_interface<void()>::sink_type;
	typedef typename executor_interface<void()>::start_result start_result;

	enum ResumErrorCode
	{
		NotRunning,
		NotResumable
	};
	typedef error<ResumErrorCode> resume_error;
	typedef ddk::result<void,resume_error> resume_result;

	virtual start_result execute(const ddk::function<void()>&,const sink_type&) = 0;
	virtual resume_result resume() = 0;
	virtual void signal() = 0;
};

typedef unique_reference_wrapper<context_executor_interface> context_executor_unique_ref;
typedef unique_reference_wrapper<const context_executor_interface> context_executor_const_unique_ref;
typedef unique_pointer_wrapper<context_executor_interface> context_executor_unique_ptr;
typedef unique_pointer_wrapper<const context_executor_interface> context_executor_const_unique_ptr;

typedef lent_reference_wrapper<context_executor_interface> context_executor_lent_ref;
typedef lent_reference_wrapper<const context_executor_interface> context_executor_const_lent_ref;
typedef lent_pointer_wrapper<context_executor_interface> context_executor_lent_ptr;
typedef lent_pointer_wrapper<const context_executor_interface> context_executor_const_lent_ptr;

template<typename Executor,typename ... Args>
unique_reference_wrapper<Executor> make_executor(Args&& ... i_args)
{
	typedef fixed_size_or_allocator<system_allocator> allocator_t;
	static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_unique_allocation<Executor,allocator_t>());
	
	return (s_allocator) ? ddk::make_unique_reference<Executor>(allocator_t{ *s_allocator,system_allocator{} },std::forward<Args>(i_args) ...)
						 : ddk::make_unique_reference<Executor>(std::forward<Args>(i_args) ...);
}

}
