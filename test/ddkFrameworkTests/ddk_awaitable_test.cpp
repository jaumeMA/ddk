#include <gtest/gtest.h>
#include "ddk_awaitable.h"
#include "ddk_thread_pool.h"
#include "ddk_pool_stack_allocator.h"
#include "ddk_dynamic_stack_allocator.h"

using namespace testing;

class DDKAwaitableTest: public Test
{
};

int compute(int a)
{
	int b = a;

	while(a < 50)
	{
		++a;

		ddk::pause();
	}

	return b;
}

TEST(DDKAwaitableTest,voidAwait)
{
	// auto stackAllocator = ddk::make_distributed_reference<ddk::detail::pool_stack_allocator>(ddk::make_distributed_reference<ddk::detail::default_dynamic_stack_allocator>(),10,10);
	// ddk::mutex _mutex;
	// ddk::awaitable<int> newAwaitable = ddk::awaitable<int>(ddk::make_function(compute,1),stackAllocator);
	// ddk::thread_pool threadPool(ddk::thread_pool::FixedSize,100);
	// if(auto acquireRes = threadPool.acquire_sheaf(60))
	// {
	// 	bool done = false;
	// 	ddk::thread_sheaf threadSheaf = std::move(acquireRes).extract();

	// 	threadSheaf.start([&]()
	// 	{
	// 		ddk::mutex_guard mg(_mutex);

	// 		if(!done)
	// 		{
	// 			if(ddk::awaited_result<int> res = resume(newAwaitable))
	// 			{
	// 				done = true;
	// 			}
	// 		}
	// 	}).dismiss();

	// 	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	// }
}