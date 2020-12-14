#include <gtest/gtest.h>
#include "ddk_async.h"
#include "ddk_fiber_pool.h"
#include "ddk_async.h"
#include "test_utils.h"
#include "ddk_high_order_array.h"

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
        ++s_counter;

		if(s_counter < 1000)
		{
			ddk::yield();
		}
		else
		{
			break;
		}
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
			break;
		}

		++s_counter;
	}
}

TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFunc)
{
	{
		ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10);
		ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

		if(acquireRes == ddk::success)
		{
			ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
			ddk::future<void> provaFuture = ddk::async(light_func)->attach(std::move(fiberSheaf));

			provaFuture.wait();
		}
	}
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFuncStoredInPromise)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(1);

	if(acquireRes == ddk::success)
	{
		ddk::promise<void> prom;
		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
		ddk::future<void> provaFuture = ddk::async(recursive_func) -> store(prom)-> attach(std::move(fiberSheaf));

		const ddk::future<void>::cancel_result cancelRes = provaFuture.cancel();

		if (cancelRes != ddk::success)
		{
			provaFuture.wait();
		}
	}
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstHeavyFunc)
{
	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes == ddk::success)
	{
		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
		ddk::future<void> provaFuture = ddk::async(heavy_func) -> attach(std::move(fiberSheaf));

		provaFuture.wait();
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstRecursiveFunc)
{
	ddk::future<int> myFuture = ddk::async(ddk::make_function([](){ printf("funcio oroginal\n"); return 0; }));

	//ddk::future<int> myFuture2 = ddk::async(ddk::make_function([]() { return 0; }));
	//ddk::shared_future<int> mySharedFuture = share(std::move(myFuture2));

	ddk::thread myThread;
	ddk::future<char> myOtherFuture = std::move(myFuture)
	.then(ddk::make_function([](const int& i_value)
	{ 
		printf("executada la primera part: %d\n", i_value);

		return std::string("hola");
	}))
	.on_error(ddk::make_function([](const ddk::async_exception&)
	{ 
		printf("ep, exception\n"); 
	}))
	.then_on(ddk::make_function([](const std::string& i_value) 
	{
		printf("executada la segon part: %s\n",i_value.c_str());

		throw std::exception{ "hola nens, aixo es una excepcio" };
												
		return i_value[0]; 
	}),std::move(myThread))
	.on_error(ddk::make_function([](const ddk::async_exception&) 
	{ 
		printf("ep, segona exception\n"); 
	}));

	try
	{
		char res = myOtherFuture.extract_value();
	}
	catch(...)
	{
	}

	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);
	if(acquireRes == ddk::success)
	{
		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
		ddk::future<void> provaFuture = ddk::async(recursive_func) -> attach(std::move(fiberSheaf));

		provaFuture.wait();
		provaFuture.extract_value();
	}
}
