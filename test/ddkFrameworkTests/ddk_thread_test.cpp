#include <gtest/gtest.h>
#include "ddk_thread.h"
#include "test_utils.h"
#include "ddk_thread_utils.h"

using namespace testing;
using testing::Types;

class DDKThreadTest : public Test
{
};

class ThreadCallee
{
public:
	ThreadCallee() = default;
	void run()
	{
		m_running = true;

		printf("Executing thread callback");

		while(m_stop == false)
		{
			printf(".");

			ddk::sleep(100);
		}

		printf("\n");

		m_running = false;
	}
	void stop(bool i_stop = true)
	{
		m_stop = i_stop;
	}
	bool is_stopped() const
	{
		return m_stop;
	}
	bool is_running() const
	{
		return m_running;
	}

private:
	bool m_stop = false;
	bool m_running = false;
};

TEST(DDKThreadTest,defaultConstruction)
{
	ddk::thread tmp;
}
TEST(DDKThreadTest,threadRun)
{
	ThreadCallee callee;
	ddk::thread tmp;

	EXPECT_EQ(static_cast<bool>(tmp.start(ddk::make_function(&callee,&ThreadCallee::run))),true);

	ddk::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	EXPECT_EQ(static_cast<bool>(tmp.stop()),true);
}
TEST(DDKThreadTest,threadReRun)
{
	ThreadCallee callee;
	ddk::thread tmp;

	EXPECT_EQ(static_cast<bool>(tmp.start(ddk::make_function(&callee,&ThreadCallee::run))),true);

	ddk::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	EXPECT_EQ(static_cast<bool>(tmp.stop()),true);

	EXPECT_EQ(callee.is_running(),false);

	callee.stop(false);

	EXPECT_EQ(static_cast<bool>(tmp.start(ddk::make_function(&callee,&ThreadCallee::run))),true);

	ddk::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	EXPECT_EQ(static_cast<bool>(tmp.stop()),true);
}