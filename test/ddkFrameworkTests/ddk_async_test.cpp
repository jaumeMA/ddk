#include <gtest/gtest.h>
#include "ddk_async.h"
#include "ddk_fiber_pool.h"
#include "ddk_async.h"
#include "test_utils.h"
#include "ddk_high_order_array.h"
#include <stdexcept>

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
	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

	if(acquireRes == ddk::success)
	{
		ddk::promise<void> prom;
		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
		ddk::future<void> provaFuture = ddk::async(recursive_func) -> store(prom) -> attach(std::move(fiberSheaf));

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
TEST(DDKAsyncTest,asyncNonCopiablePayload)
{
	enum class ErrorCode
	{
		Error
	};
	typedef ddk::result<ddk::unique_reference_wrapper<int>,ErrorCode> my_result;

	ddk::thread myThread;

	ddk::async(ddk::make_function([]() -> my_result { return ddk::make_unique_reference<int>(10); })) -> attach(std::move(myThread))
	.then(ddk::make_function([](my_result i_value)
	{
		if(i_value)
		{
			return true;
		}
		else
		{
			return false;
		}
	}));

	ddk::thread _myThread;

	ddk::async(ddk::make_function([]() -> ddk::future<my_result> { return ddk::async(ddk::make_function([]() -> my_result { return ddk::make_unique_reference<int>(10); })); }))->attach(std::move(_myThread))
	.then(ddk::make_function([](my_result i_value)
	{
		if(i_value)
		{
			ddk::unique_reference_wrapper<int> nestedRes = std::move(i_value).extract();

			return true;
		}
		else
		{
			return false;
		}
	}));
}
TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstRecursiveFunc)
{
	ddk::future<int> myFuture = ddk::async(ddk::make_function([](){ printf("funcio oroginal\n"); return 0; }));

	//ddk::future<int> myFuture2 = ddk::async(ddk::make_function([]() { return 0; }));
	//ddk::shared_future<int> mySharedFuture = share(std::move(myFuture2));

	ddk::thread myThread;
	ddk::thread myOtherThread;
	ddk::future<const char*> myOtherFuture = std::move(myFuture)
	.then(ddk::make_function([](const int& i_value)
	{
		return std::string("hola");
	}))
	.on_error(ddk::make_function([](const ddk::async_error&)
	{
	}))
	.then_on(ddk::make_function([](const std::string& i_value)
	{
		throw std::runtime_error{ "hola nens, aixo es una excepcio" };

		return i_value[0];
	}),std::move(myThread))
	.on_error(ddk::make_function([](const ddk::async_error&)
	{
	}))
	.async(ddk::make_function([](const char& i_value)
	{
		return 10;
	}),std::move(myOtherThread))
	.then(ddk::make_function([](const int& i_value)
	{
		return "hola";
	}));

	ddk::future<ddk::values_array<const char*,4>> res = std::move(myOtherFuture) && std::move(myOtherFuture) && std::move(myOtherFuture) && std::move(myOtherFuture);

	try
	{
		const ddk::values_array<const char*,4> resStr = res.extract_value();
		int a = 0;
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
