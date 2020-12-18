#include <gtest/gtest.h>
#include "ddk_async_signal.h"
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

TEST(DDKAsyncSignalTest,defaultConstruction)
{
	ddk::async_signal<void(int,char,const std::string&)> tmp;

	AsyncSignalCallee callee;

	ddk::connection tmpConn = tmp.connect(ddk::make_function(&callee,&AsyncSignalCallee::memberCall));

	tmp.execute(10,'a',"hola");

	ddk::sleep(5000);

	EXPECT_EQ(callee.memberCalled,true);
}