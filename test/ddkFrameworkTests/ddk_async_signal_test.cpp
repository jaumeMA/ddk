#include <gtest/gtest.h>
#include "ddk_async_signal.h"
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

//TEST(DDKAsyncSignalTest,defaultConstruction)
//{
//	typedef ddk::Id<size_t,size_t> myId;
//	std::map<myId,myId> kk;
//
//	cucu(get_cucu(10),get_cucu(20));
//
//	kk[myId{ 10 }] = myId{ 20 };
//	kk[myId{ 30 }] = myId{ 40 };
//	kk[myId{ 50 }] = myId{ 60 };
//
//	const std::string kkk = ddk::to_string(kk);
//
//	std::map<myId,myId> kkkk = ddk::formatter<std::map<myId,myId>>::format(kkk);
//
//	int a = 0;
//
//	ddk::async_signal<void(int,char,const std::string&)> tmp;
//
//	AsyncSignalCallee callee;
//
//	ddk::connection tmpConn = tmp.connect(ddk::make_function(&callee,&AsyncSignalCallee::memberCall));
//
//	tmp.execute(10,'a',"hola");
//
//	ddk::sleep(5000);
//
//	EXPECT_EQ(callee.memberCalled,true);
//
//	ddk::thread_sheaf_event_driven_executor _executor;
//	ddk::fiber_sheaf_event_driven_executor __executor;
//}
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