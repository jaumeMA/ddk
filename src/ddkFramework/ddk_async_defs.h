//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_result.h"
#include "ddk_type_id.h"
#include "ddk_scoped_enum.h"

namespace ddk
{

SCOPED_ENUM_DECL(ExecutorState,
	Idle,
	Executing,
	Executed = Idle,
	Cancelling = Executing + 1,
	Cancelled = Cancelling + 1
);

namespace detail
{

enum StartErrorCode
{
	StartNoContext,
	StartNotExecutable,
	StartNotAvailable
};
typedef error<StartErrorCode> start_error;
typedef result<ExecutorState,start_error> start_result;

enum CancelErrorCode
{
	CancelNoContext,
	CancelAlreadyExecuted
};
typedef error<CancelErrorCode> cancel_error;
typedef result<void,cancel_error> cancel_result;

}

struct sender_id_t;
typedef Id<size_t,sender_id_t> sender_id;

struct receiver_id_t;
typedef Id<size_t,receiver_id_t> receiver_id;

static const unsigned long k_messageQueueUpdateTimeInMS = 500;

SCOPED_ENUM_DECL(SchedulerPolicy,
	None,
	FireAndForget,
	FireAndReuse
);

template<typename Payload>
struct async_event
{
public:
	async_event() = default;
	async_event(const async_event&) = delete;
	async_event(async_event&&) = default;
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Payload,Args...))
	async_event(Args&& ... i_args);
	Payload extract() &&;
	operator bool() const;

private:
	optional<Payload> m_payload;
};

}

#include "ddk_async_defs.inl"