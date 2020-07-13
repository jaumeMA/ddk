#include <gtest/gtest.h>
#include "ewas_signal.h"
#include "test_utils.h"

using namespace testing;
using testing::Types;

class EwasSignalTest : public Test
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

TEST(EwasSignalTest,defaultConstruction)
{
	ewas::signal<void()> tmp;
}
TEST(EwasSignalTest,signalToFreeFunctionConnect)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(EwasSignalTest,signalToStaticFunctionConnect)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(&SignalCallee::staticCall);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(EwasSignalTest,signalToMemberFunctionConnect)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	SignalCallee callee;
	ewas::connection tmpConn = tmp.connect(&callee,&SignalCallee::memberCall);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);
}
TEST(EwasSignalTest,signalDisable)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disable();

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);
}
TEST(EwasSignalTest,signalReenable)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

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
TEST(EwasSignalTest,signalDisconnect)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);
	EXPECT_EQ(tmpConn.is_enabled(),true);

	tmpConn.disconnect();

	EXPECT_EQ(tmpConn.is_connected(),false);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,false);
}
TEST(EwasSignalTest,signalReconnect)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

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
TEST(EwasSignalTest,signalExecute)
{
	ewas::signal<void(int,char,const std::string&)> tmp;

	ewas::connection tmpConn = tmp.connect(freeFunction);

	EXPECT_EQ(tmpConn.is_connected(),true);

	freeFunctionCalled = false;

	tmp.execute(0,'a',"");

	EXPECT_EQ(freeFunctionCalled,true);
}