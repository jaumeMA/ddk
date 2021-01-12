#include "ddk_sync_executor.inl"

namespace ddk
{

template<typename Return>
async_shared_ref<Return> async(const ddk::function<Return()>& i_function)
{
	async_shared_ref<Return> asyncExecutor = make_async_executor(i_function);

	return asyncExecutor;
}

}
