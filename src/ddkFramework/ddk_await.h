#pragma once

#include "ddk_awaitable.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename Return, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function);

}

#include "ddk_await.inl"
