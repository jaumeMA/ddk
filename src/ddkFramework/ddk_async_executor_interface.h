#pragma once

#include "ddk_executor_interface.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_embedded_type.h"

namespace ddk
{

template<typename Return>
class async_interface_base
{
public:
	virtual ~async_interface_base() = default;
};

template<typename Return>
using async_base_shared_ref = shared_reference_wrapper<async_interface_base<Return>>;
template<typename Return>
using async_base_const_shared_ref = shared_reference_wrapper<const async_interface_base<Return>>;
template<typename Return>
using async_base_shared_ptr = shared_pointer_wrapper<async_interface_base<Return>>;
template<typename Return>
using async_base_const_shared_ptr = shared_pointer_wrapper<const async_interface_base<Return>>;

template<typename Return>
class async_state_interface : public async_interface_base<Return>
{
public:
	typedef async_state_interface<Return> async_public_type;
	typedef typename embedded_type<Return>::ref_type reference;
	typedef typename embedded_type<Return>::cref_type const_reference;
	typedef typename embedded_type<Return>::raw_type value_type;

	virtual void wait() const = 0;
	virtual void wait_for(unsigned int i_period) const = 0;
	virtual reference get_value() = 0;
	virtual const_reference get_value() const = 0;
	virtual value_type extract_value() = 0;
	virtual bool ready() const = 0;
};

template<>
class async_state_interface<void> : public async_interface_base<void>
{
public:
	typedef async_state_interface<detail::void_t> async_public_type;

	virtual void wait() const = 0;
	virtual void wait_for(unsigned int i_period) const = 0;
	virtual bool ready() const = 0;
};

template<typename Return>
using async_state_shared_ref = shared_reference_wrapper<typename async_state_interface<Return>::async_public_type>;
template<typename Return>
using async_state_const_shared_ref = shared_reference_wrapper<const typename async_state_interface<Return>::async_public_type>;
template<typename Return>
using async_state_shared_ptr = shared_reference_wrapper<typename async_state_interface<Return>::async_public_type>;
template<typename Return>
using async_state_const_shared_ptr = shared_reference_wrapper<const typename async_state_interface<Return>::async_public_type>;

template<typename Return>
class async_cancellable_interface : public async_state_interface<Return>
{
public:
	typedef typename executor_interface<Return()>::cancel_result cancel_result;

	virtual cancel_result cancel() = 0;
};

template<typename Return>
using async_cancellable_shared_ref = shared_reference_wrapper<async_cancellable_interface<Return>>;
template<typename Return>
using async_cancellable_const_shared_ref = shared_reference_wrapper<const async_cancellable_interface<Return>>;
template<typename Return>
using async_cancellable_shared_ptr = shared_pointer_wrapper<async_cancellable_interface<Return>>;
template<typename Return>
using async_cancellable_const_shared_ptr = shared_pointer_wrapper<const async_cancellable_interface<Return>>;

template<typename Return>
class async_execute_interface : public async_cancellable_interface<Return>
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

}