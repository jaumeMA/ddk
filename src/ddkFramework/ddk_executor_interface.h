#pragma once

#include <functional>
#include "unique_reference_wrapper.h"
#include "lent_reference_wrapper.h"
#include "reference_wrapper.h"
#include "ddk_result.h"

namespace ddk
{
namespace detail
{

template<typename RReturn>
struct function_type
{
    typedef std::function<void(RReturn)> type;
};
template<>
struct function_type<void>
{
    typedef std::function<void()> type;
};

}

template<typename>
class executor_interface;

SCOPED_ENUM_DECL(ExecutorState,
	Idle,
	Executing,
	Executed
);

template<typename Return, typename ... Args>
class executor_interface<Return(Args...)>
{
public:
	enum StartErrorCode
	{
		NoCallable,
		NotAvailable
	};

	typedef result<ExecutorState,StartErrorCode> start_result;

	virtual ~executor_interface() = default;
	virtual ExecutorState get_state() const = 0;
	virtual start_result execute(const typename detail::function_type<Return>::type&, const std::function<Return(Args...)>&, Args ...) = 0;
};

template<typename Executor, typename ... Args>
unique_reference_wrapper<Executor> make_executor(Args&& ... i_args)
{
	return ewas::make_unique_reference<Executor>(std::forward<Args>(i_args) ...);
}

template<typename Return, typename ... Args>
using executor_unique_ref = unique_reference_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_unique_ref = unique_reference_wrapper<const executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_unique_ptr = unique_pointer_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_unique_ptr = unique_pointer_wrapper<const executor_interface<Return(Args...)>>;

template<typename Return, typename ... Args>
using executor_lent_ref = lent_reference_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_lent_ref = lent_reference_wrapper<const executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_lent_ptr = lent_pointer_wrapper<executor_interface<Return(Args...)>>;
template<typename Return, typename ... Args>
using executor_const_lent_ptr = lent_pointer_wrapper<const executor_interface<Return(Args...)>>;

}
