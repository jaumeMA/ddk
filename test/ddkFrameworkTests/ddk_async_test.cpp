#include <gtest/gtest.h>
#include "ddk_async.h"
#include "ddk_fiber_pool.h"
#include "ddk_async.h"
#include "ddk_extack_allocator.h"
#include "ddk_iterable.h"
#include "test_utils.h"
#include "ddk_high_order_array.h"
#include <stdexcept>

using namespace testing;

class DDKAsyncTest : public Test
{
};

class VariadicClass
{
public:
	VariadicClass(int a,std::string b)
	{
	}
};

template<typename T>
class MyPromise : public ddk::executor_promise<T,ddk::system_allocator>
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
			ddk::suspend();
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
	printf("1\n");
	HeaveyClass myClass1(0xff);
	printf("2\n");
	HeaveyClass myClass2(0xff);
	printf("3\n");
	HeaveyClass myClass3(0xff);
	printf("4\n");
	HeaveyClass myClass4(0xff);
	printf("5\n");
	HeaveyClass myClass5(0xff);
	printf("6\n");
	HeaveyClass myClass6(0xff);
	printf("7\n");

	while(true)
	{
		if(s_counter < 20)
		{
			ddk::yield();
			printf("%d\n",s_counter);
		}
		else
		{
			break;
		}

		++s_counter;
	}
}

// TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFunc)
// {
// 	{
// 		{
// 			ddk::future<std::string> _ = ddk::async([]() -> std::string
// 			{
// 				return "HOLA";
// 			});
// 			std::move(_).then([](const std::string& i_str)
// 			{
// 				printf("%s\n",i_str.c_str());
// 			});
// 		}

// 		ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10);
// 		ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

// 		if(acquireRes == ddk::success)
// 		{
// 			ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
// 			ddk::future<void> provaFuture = ddk::async([]() { light_func(); }) -> attach(std::move(fiberSheaf));

// 			provaFuture.wait();
// 		}
// 	}
// }
// TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstLightFuncStoredInPromise)
// {
// 	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
// 	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

// 	if(acquireRes == ddk::success)
// 	{
// 		ddk::promise<VariadicClass> kk(10,"hola");
// 		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
// 		ddk::future<void> provaFuture = ddk::async([&]() 
// 		{
// 			recursive_func();
// 		}) -> store<MyPromise<ddk::detail::void_t>>() 
// 		   -> attach(std::move(fiberSheaf));

// 		const ddk::future<void>::cancel_result cancelRes = provaFuture.cancel();

// 		if (cancelRes != ddk::success)
// 		{
// 			provaFuture.wait();
// 		}
// 	}
// }
// TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstHeavyFunc)
// {
// 	ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
// 	ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);

// 	if(acquireRes == ddk::success)
// 	{
// 		ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
// 		ddk::future<void> provaFuture = ddk::async([]() {heavy_func();}) -> attach(std::move(fiberSheaf));

// 		provaFuture.wait();
// 	}

// 	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
// }
// TEST(DDKAsyncTest,asyncNonCopiablePayload)
// {
// 	enum class ErrorCode
// 	{
// 		Error
// 	};
// 	typedef ddk::result<ddk::unique_reference_wrapper<int>,ErrorCode> my_result;

// 	ddk::thread myThread;
// 	// int step = 0;

// 	//ddk::async([]() {}) -> attach(std::move(myThread));

// 	// //ddk::async([&]() -> my_result 
// 	// //{ 
// 	// //	step += 1; return ddk::make_unique_reference<int>(10); 
// 	// //}) -> attach(std::move(myThread))
// 	// //.then([&](my_result i_value)
// 	// //{
// 	// //	step += 2;
// 	// //	if(i_value)
// 	// //	{
// 	// //		return true;
// 	// //	}
// 	// //	else
// 	// //	{
// 	// //		return false;
// 	// //	}
// 	// //})
// 	// //.then([&](bool i_result)
// 	// //{
// 	// //	step += 3;
// 	// //});

// 	ddk::thread _myThread;
// 	ddk::thread myOtherThread;
// 	int res = 0;

// 	ddk::async([&]() -> my_result
// 	{ 
// 	 	return ddk::make_unique_reference<int>(10); 
// 	}) -> attach(std::move(_myThread))
// 	.then([_myThread = std::move(myThread)](my_result i_value) mutable -> ddk::future<std::string>
// 	{
// 		if (i_value)
// 		{
// 			return ddk::async([]() { return std::string("sucess"); }) -> attach(std::move(_myThread));
// 		}
// 		else
// 		{
// 			return ddk::async([]() { return std::string("error"); }) -> attach(std::move(_myThread));
// 		}
// 	})
// 	.then([_myOtherThread = std::move(myOtherThread)](std::string i_value) mutable -> ddk::future<std::string>
// 	{
// 		return ddk::async([i_value]() { return i_value + std::string(":sucess"); }) -> attach(std::move(_myOtherThread));
// 	})
// 	.then([](std::string i_value)
// 	{
// 		printf("RESULT: %s\n",i_value.c_str());

// 		return 10;
// 	});

// 	printf("DONE\n");
// }
// TEST(DDKAsyncTest, asyncExecByFiberPoolAgainstRecursiveFunc)
// {
// 	// //std::this_thread::sleep_for(std::chrono::seconds(1000000));

// 	ddk::thread myThread;
// 	{
// 		int a = 0;
// 		ddk::future<int&> res = ddk::async([counter = 0]() mutable { return counter++; })
// 		-> schedule<ddk::polling_async_scheduler>()
// 		-> attach(std::move(myThread))
// 		.then([&](int i_value) -> int&
// 		{
// 			printf("CURRENT VALUE: %d\n",i_value);

// 			return a;
// 		});

// 		std::this_thread::sleep_for(std::chrono::seconds(3));

// 		res.cancel().dismiss();
// 	}

// 	ddk::thread _myThread;
// 	ddk::future<char> myFuture = ddk::async<ddk::system_extack_allocator>(ddk::make_function([]() { printf("funcio oroginal\n"); return 'a'; }),local_allocator(1024)) -> attach(std::move(_myThread))
// 	.then([](char i_value)
// 	{
// 		printf("valor rebut %c\n",i_value);

// 		return i_value;
// 	});

// 	// //ddk::future<int> myFuture2 = ddk::async(ddk::make_function([]() { return 0; }));
// 	// //ddk::shared_future<int> mySharedFuture = share(std::move(myFuture2));

// 	ddk::thread myOtherThread;
// 	// ddk::future<int> myOtherFuture = ddk::async(ddk::make_function([]() { printf("funcio oroginal\n"); return 10; })) -> attach(std::move(myOtherThread));

// 	ddk::thread myOtherOtherThread;
// 	ddk::future<int> myOtherOtherFuture = ddk::async(ddk::make_function([]() { printf("funcio oroginal\n"); return 237; }))->attach(std::move(myOtherOtherThread));
// 	ddk::future<const char*> myOtherFuture = std::move(myFuture)
// 	.then(ddk::make_function([](const int& i_value)
// 	{
// 		return std::string("hola");
// 	}))
// 	.on_error([](const ddk::async_error&)
// 	{
// 	})
// 	.then_on([](const std::string& i_value)
// 	{
// 		return i_value[0];
// 	},std::move(myThread))
// 	.on_error(ddk::make_function([](const ddk::async_error&)
// 	{
// 	}))
// 	.async(ddk::make_function([](const char& i_value)
// 	{
// 		return 10;
// 	}),std::move(myOtherThread))
// 	.then(ddk::make_function([](const int& i_value)
// 	{
// 		return "hola";
// 	}));

// 	ddk::future<int> kk = compose(std::move(myFuture), std::move(myOtherFuture))
// 	.then([](std::tuple<char,const char*> i_data)
// 	{
// 		return static_cast<int>(reinterpret_cast<size_t>(std::get<1>(i_data)));
// 	});

// 	compose(std::move(myOtherOtherFuture),std::move(kk))
// 	.then([](std::array<int, 2> i_data)
// 	{
// 		return static_cast<float>(i_data[0] + i_data[1]);
// 	})
// 	.then([](float i_data)
// 	{
// 		int a = static_cast<int>(i_data);
// 		++a;
// 	});

// 	// ddk::fiber_pool fiberPool(ddk::fiber_pool::FixedSize,10,25);
// 	// ddk::fiber_pool::acquire_result<ddk::fiber_sheaf> acquireRes = fiberPool.acquire_sheaf(10);
// 	// if(acquireRes == ddk::success)
// 	// {
// 	// 	ddk::fiber_sheaf fiberSheaf = std::move(acquireRes).extract();
// 	// 	ddk::future<void> provaFuture = ddk::async([]() { recursive_func(); }) -> attach(std::move(fiberSheaf));

// 	// 	provaFuture.wait();
// 	// 	std::move(provaFuture).extract_value();
// 	// }
// }
