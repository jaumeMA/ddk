#include <gtest/gtest.h>
#include "ewas_thread.h"
#include "test_utils.h"
#include "thread_utils.h"

using namespace testing;
using testing::Types;

class EwasThreadTest : public Test
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

			ewas::sleep(100);
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

TEST(EwasThreadTest,defaultConstruction)
{
	ewas::thread tmp;
}
TEST(EwasThreadTest,threadRun)
{
	ThreadCallee callee;
	ewas::thread tmp;

	tmp.start(std::bind(&ThreadCallee::run,&callee));

	ewas::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	tmp.stop();
}
TEST(EwasThreadTest,threadReRun)
{
	ThreadCallee callee;
	ewas::thread tmp;

	tmp.start(std::bind(&ThreadCallee::run,&callee));

	ewas::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	tmp.stop();

	EXPECT_EQ(callee.is_running(),false);

	callee.stop(false);

	tmp.start(std::bind(&ThreadCallee::run,&callee));

	ewas::sleep(1000);

	EXPECT_EQ(callee.is_running(),true);

	callee.stop();

	tmp.stop();	
}