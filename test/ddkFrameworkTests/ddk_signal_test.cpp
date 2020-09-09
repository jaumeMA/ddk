#include <gtest/gtest.h>
#include "ddk_signal.h"
#include "test_utils.h"

using namespace testing;
using testing::Types;

class DDKSignalTest : public Test
{
};

class SignalCallee
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
bool SignalCallee::functionCalled = false;

bool freeFunctionCalled = false;

void freeFunction(int,char,const std::string&)
{
	freeFunctionCalled = true;
}

TEST(DDKSignalTest,defaultConstruction)
{
	ddk::signal<void()> tmp;
}
TEST(DDKSignalTest,signalToFreeFunctionConnect)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(DDKSignalTest,signalToStaticFunctionConnect)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(&SignalCallee::staticCall);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(DDKSignalTest,signalToMemberFunctionConnect)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	SignalCallee callee;
	ddk::connection tmpConn = tmp.connect(ddk::make_function(&callee,&SignalCallee::memberCall));

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(DDKSignalTest,signalDisable)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disable();

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);
}
TEST(DDKSignalTest,signalReenable)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disable();

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);

	tmpConn.enable();

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,true);
}
TEST(DDKSignalTest,signalDisconnect)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disconnect();

	EXPECT_EQ(tmpConn.is_connected(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);
}
TEST(DDKSignalTest,signalReconnect)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disconnect();

	EXPECT_EQ(tmpConn.is_connected(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);

	tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,true);
}
TEST(DDKSignalTest,signalExecute)
{
	ddk::signal<void(int,char,const std::string&)> tmp;

	ddk::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,true);
}