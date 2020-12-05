#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_embedded_type.h"
#include "ddk_result.h"

namespace ddk
{

class async_interface_base
{
public:
	virtual ~async_interface_base() = default;

	shared_pointer_wrapper<async_interface_base> _nextChainedAsyncOp;

	virtual bool notify() = 0;
};

using async_base_shared_ref = shared_reference_wrapper<async_interface_base>;
using async_base_const_shared_ref = shared_reference_wrapper<const async_interface_base>;
using async_base_shared_ptr = shared_pointer_wrapper<async_interface_base>;
using async_base_const_shared_ptr = shared_pointer_wrapper<const async_interface_base>;

class async_cancellable_interface : public async_interface_base
{
public:
	enum CancelErrorCode
	{
		CancelNoAsync,
		CancelNoCallable,
		CancelAlreadyExecuted
	};
	typedef result<void,CancelErrorCode> cancel_result;

	virtual cancel_result cancel() = 0;
};

using async_cancellable_shared_ref = shared_reference_wrapper<async_cancellable_interface>;
using async_cancellable_const_shared_ref = shared_reference_wrapper<const async_cancellable_interface>;
using async_cancellable_shared_ptr = shared_pointer_wrapper<async_cancellable_interface>;
using async_cancellable_const_shared_ptr = shared_pointer_wrapper<const async_cancellable_interface>;

template<typename Return>
class async_execute_interface
{
public:
	enum StartErrorCode
	{
		NotAvailable,
		AlreadyStarted,
		AlreadyDone
	};
	typedef result<void,StartErrorCode> start_result;

	virtual start_result execute() = 0;
};

template<typename Return>
using async_execute_shared_ref = shared_reference_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_shared_ref = shared_reference_wrapper<const async_execute_interface<Return>>;
template<typename Return>
using async_execute_shared_ptr = shared_pointer_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_shared_ptr = shared_pointer_wrapper<const async_execute_interface<Return>>;

template<typename Return>
using async_execute_lent_ref = lent_reference_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_lent_ref = lent_reference_wrapper<const async_execute_interface<Return>>;
template<typename Return>
using async_execute_lent_ptr = lent_pointer_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_lent_ptr = lent_pointer_wrapper<const async_execute_interface<Return>>;

}
