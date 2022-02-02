#pragma once

#include "ddk_executor_interface.h"

namespace ddk
{

class context_executor_interface : public executor_interface<void()>
{
public:
	typedef typename executor_interface<void()>::start_result start_result;

	enum ResumErrorCode
	{
		NotRunning,
		NotResumable
	};
	typedef error<ResumErrorCode> resume_error;
	typedef ddk::result<void,resume_error> resume_result;

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

}