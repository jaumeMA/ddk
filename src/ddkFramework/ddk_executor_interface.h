#pragma once

#include "ddk_function.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_executor_interface.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct sink_type_resolver
{
	typedef typename mpl::static_if<std::is_reference<T>::value,T,const T&>::type reference;	
	typedef function<void(reference)> type;
};
template<>
struct sink_type_resolver<void>
{
	typedef void reference;
	typedef function<void()> type;
};

}

template<typename>
class executor_interface;
template<typename>
class cancellable_executor_interface;

SCOPED_ENUM_DECL(ExecutorState,
	Idle,
	Executing,
	Executed,
	Cancelling,
	Cancelled
);

template<typename Return>
class executor_interface<Return()>
{
public:
	enum StartErrorCode
	{
		StartNoCallable,
		StartNotAvailable
	};
	typedef result<ExecutorState,StartErrorCode> start_result;
	typedef typename detail::sink_type_resolver<Return>::type sink_type;
	typedef typename detail::sink_type_resolver<Return>::reference sink_reference;

	virtual ~executor_interface() = default;
	virtual ExecutorState get_state() const = 0;
	virtual start_result execute(const sink_type&, const ddk::function<Return()>&) = 0;
};

template<typename Return, typename ... Args>
using executor_unique_ref = unique_reference_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_unique_ref = unique_reference_wrapper<const executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_unique_ptr = unique_pointer_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_unique_ptr = unique_pointer_wrapper<const executor_interface<Return(Args...)>>;

template<typename Return>
class cancellable_executor_interface<Return()> : public executor_interface<Return()>
{
public:
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	typedef typename async_cancellable_interface::CancelErrorCode CancelErrorCode;

	virtual cancel_result cancel(const ddk::function<bool()>&) = 0;
};

template<typename Return, typename ... Args>
using cancellable_executor_unique_ref = unique_reference_wrapper<cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_const_unique_ref = unique_reference_wrapper<const cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_unique_ptr = unique_pointer_wrapper<cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_const_unique_ptr = unique_pointer_wrapper<const cancellable_executor_interface<Return()>>;

template<typename Executor, typename ... Args>
unique_reference_wrapper<Executor> make_executor(Args&& ... i_args)
{
	return ddk::make_unique_reference<Executor>(std::forward<Args>(i_args) ...);
}

}
