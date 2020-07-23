#include <gtest/gtest.h>
#include "ddk_async.h"
#include "ddk_fiber_pool.h"
#include "ddk_async.h"
#include "test_utils.h"

using namespace testing;

class DDKAsyncTest : public Test
{
};

class HeaveyClass : public ConstructionDeletionBalancer
{
public:
	using ConstructionDeletionBalancer::ConstructionDeletionBalancer;

	~HeaveyClass()
	{
	}
	char prova[8192] = { 0 };
};

void light_func()
{
	static size_t s_counter = 0;

	while(true)
	{
		if(s_counter < 20)
		{
			ddk::yield();
		}
		else
		{
			ddk::suspend();
		}

        ++s_counter;
	}
}

void _recursive_func(int i)
{
	char _[256] = { 0 };

	if(i > 0)
	{
        ddk::yield();

		_recursive_func(i - 1);
	}
    else
    {
        ddk::suspend();
    }
}
void recursive_func()
{
	_recursive_func(100);
}

void heavy_func()
{
	static size_t s_counter = 0;
	HeaveyClass myClass1(0xff);
	HeaveyClass myClass2(0xff);
	HeaveyClass myClass3(0xff);
	HeaveyClass myClass4(0xff);
	HeaveyClass myClass5(0xff);
	HeaveyClass myClass6(0xff);

	while(true)
	{
		if(s_counter < 20)
		{
			ddk::yield();
		}
		else
		{
			ddk::suspend();
		}

		++s_counter;
	}
}

TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFunc)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes.hasError() == false)
	{
		ddk::fiber_sheaf fiberSheaf = acquireRes.extractPayload();
		ddk::future<void> provaFuture = ddk::async(light_func) -> attach(std::move(fiberSheaf));

		provaFuture.wait();
	}
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFuncStoredInPromise)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes.hasError() == false)
	{
		ddk::promise<void> prom;
		ddk::fiber_sheaf fiberSheaf = acquireRes.extractPayload();
		ddk::future<void> provaFuture = ddk::async(recursive_func) -> attach(std::move(fiberSheaf)) -> store(prom);

		provaFuture.wait();
	}
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstHeavyFunc)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes.hasError() == false)
	{
		ddk::fiber_sheaf fiberSheaf = acquireRes.extractPayload();
		ddk::future<void> provaFuture = ddk::async(heavy_func) -> attach(std::move(fiberSheaf));

		provaFuture.wait();
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstRecursiveFunc)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes.hasError() == false)
	{
		ddk::fiber_sheaf fiberSheaf = acquireRes.extractPayload();
		ddk::future<void> provaFuture = ddk::async(recursive_func) -> attach(std::move(fiberSheaf));

		provaFuture.wait();
	}
}