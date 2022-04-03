#include <gtest/gtest.h>
#include "ddk_async_signal.h"
#include "ddk_async.h"
#include "ddk_fiber_executor.h"
#include "test_utils.h"

using namespace testing;
using testing::Types;

class DDKAsyncSignalTest : public Test
{
};

class AsyncSignalCallee
{
public:

	static void staticCall(int,char,const std::string&)
	{
	}
	void memberCall(int,char,const std::string&)
	{
		memberCalled = true;
	}

	static bool functionCalled;
	bool memberCalled = false;
};
bool AsyncSignalCallee::functionCalled = false;

bool freeAsyncFunctionCalled = false;

void freeAsyncFunction(int,char,const std::string&)
{
	freeAsyncFunctionCalled = true;
}

int get_cucu(int a)
{
	return a;
}

void cucu(int a, int b)
{

}

TEST(DDKAsyncSignalTest,defaultConstruction)
{
	typedef ddk::Id<size_t,size_t> myId;
	ddk::async_signal<void(int,char,const std::string&)> tmp;

	std::map<int,int> __kkkk;
	__kkkk[0] = 10;
	__kkkk[-21] = 321;
	__kkkk[4] = 4;
	__kkkk[75] = -67;
	__kkkk[-768] = 100;

	const std::string __kkkStr = ddk::to_string(__kkkk);
	const std::map<int,int> __kkkkk = ddk::format_to<std::map<int,int>>(__kkkStr);

	std::set<int> __prova;
	__prova.insert(0);
	__prova.insert(2);
	__prova.insert(1);
	__prova.insert(7);
	__prova.insert(5);

	const std::string __provaStr = ddk::to_string(__prova);
	const std::set<int> __fromStr = ddk::format_to<std::set<int>>(__provaStr);

	AsyncSignalCallee callee;

	ddk::connection tmpConn = tmp.connect(ddk::make_function(&callee,&AsyncSignalCallee::memberCall));
	bool stop = false;

	ddk::thread myThread;
	ddk::future<void> executeFuture = ddk::async(ddk::make_function([&tmp,&stop]()
	{
		while(!stop)
		{
			tmp.execute(10,'a',"hola");
			ddk::sleep(1);
		}
	})) -> attach(std::move(myThread));

	ddk::sleep(10000);

	tmpConn.disconnect();

	ddk::sleep(10000);

	stop = true;

	executeFuture.wait();

	EXPECT_EQ(callee.memberCalled,true);

	ddk::thread_sheaf_event_driven_executor _executor;
	ddk::fiber_sheaf_event_driven_executor __executor;
}
TEST(DDKAsyncSignalTest,signalDelegate)
{
	typedef ddk::Id<size_t,size_t> myId;
	std::map<myId,myId> kk;

	cucu(get_cucu(10),get_cucu(20));

	kk[myId{ 10 }] = myId{ 20 };
	kk[myId{ 30 }] = myId{ 40 };
	kk[myId{ 50 }] = myId{ 60 };

	const std::string kkk = ddk::to_string(kk);

	std::map<myId,myId> kkkk = ddk::formatter<std::map<myId,myId>>::format(kkk);

	int a = 0;

	ddk::async_signal<void(int,char,const std::string&)> tmpOther;
	ddk::async_signal<void(int,char,const std::string&)> tmp;

	AsyncSignalCallee callee;

	ddk::connection tmpConn = tmp.connect(tmpOther);
	ddk::connection tmpOtherConn = tmpOther.connect(ddk::make_function(&callee,&AsyncSignalCallee::memberCall));

	tmp.execute(10,'a',"hola");

	ddk::sleep(5000);

	EXPECT_EQ(callee.memberCalled,true);

	ddk::thread_sheaf_event_driven_executor _executor;
	ddk::fiber_sheaf_event_driven_executor __executor;
}