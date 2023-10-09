#pragma once

#include "ddk_task_executor.h"

template<typename Iterable,typename Sink>
ddk::future<ddk::iteration_result> operator<<=(ddk::task_executor& i_lhs, ddk::iteration<Iterable,Sink>& i_rhs);

#include "ddk_iteration_utils.inl"