#pragma once

#include "ddk_function.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_executor_interface.h"
#include "ddk_sync_executor_context_interface.h"
#include "ddk_async_defs.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct sink_type_resolver
{
	typedef typename mpl::static_if<std::is_reference<T>::value,T,typename mpl::static_if<std::is_copy_constructible<T>::value,const T&,T&&>::type>::type reference;
	typedef variant<reference,async_exception> result_t;
	typedef function<void(result_t)> type;
};
template<>
struct sink_type_resolver<void>
{
	typedef detail::void_t reference;
	typedef variant<detail::void_t,async_exception> result_t;
	typedef function<void(result_t)> type;
};

}

template<typename>
class executor_interface;
template<typename>
class cancellable_executor_interface;

template<typename Return>
class executor_interface<Return()>
{
public:
	enum StartErrorCode
	{
		StartNoCallable,
		StartNotExecutable,
		StartNotAvailable
	};
	typedef error<StartErrorCode> start_error;
	typedef result<ExecutorState,start_error> start_result;
	typedef typename detail::sink_type_resolver<Return>::result_t sink_result;
	typedef typename detail::sink_type_resolver<Return>::type sink_type;
	typedef typename detail::sink_type_resolver<Return>::reference sink_reference;

	virtual ~executor_interface() = default;
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
    using typename executor_interface<Return()>::sink_type;
	typedef typename async_cancellable_interface::cancel_result cancel_result;
	typedef typename async_cancellable_interface::CancelErrorCode CancelErrorCode;
};

template<typename Return, typename ... Args>
using cancellable_executor_unique_ref = unique_reference_wrapper<cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_const_unique_ref = unique_reference_wrapper<const cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_unique_ptr = unique_pointer_wrapper<cancellable_executor_interface<Return()>>;
template<typename Return, typename ... Args>
using cancellable_executor_const_unique_ptr = unique_pointer_wrapper<const cancellable_executor_interface<Return()>>;

}
