#include "ddk_async.h"

namespace ddk
{

async_shared_ref<void> async(const ddk::function<void()>& i_function)
{
	//put here a composed callable
	async_shared_ref<void> asyncExecutor = make_async_executor(ddk::function<detail::void_t()>([i_function]() -> detail::void_t { i_function(); return _void; }));

	return asyncExecutor;
}

}