#include <gtest/gtest.h>
#include "ewas_lock_free_stack.h"
#include "test_utils.h"
#include "ewas_thread.h"
#include <array>

using namespace testing;
using testing::Types;

class EwasLockFreeStackTest : public Test
{
};

TEST(EwasLockFreeStackTest,defaultConstruction)
{
	ewas::lock_free_stack<DefaultType> lockFreeStack;

	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(EwasLockFreeStackTest,singleProducerSingleConsumer)
{
	ewas::lock_free_stack<DefaultType> lockFreeStack;

	const size_t numEntries = 1000;
	for(size_t index=0;index<numEntries;++index)
	{
		lockFreeStack.push(static_cast<int>(index));
	}

	EXPECT_EQ(lockFreeStack.empty(),false);

	size_t index = 0;
	while(true)
	{
		if(ewas::optional<DefaultType> currValue = lockFreeStack.pop())
		{
			EXPECT_EQ(*currValue,static_cast<int>(index));
		}
		else
		{
			break;
		}

		index++;
	}


	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(EwasLockFreeStackTest,singleProducerMultipleConsumer)
{
	ewas::lock_free_stack<DefaultType> lockFreeStack;
	bool stop = false;

	auto consumerFunc = [&stop,&lockFreeStack]()
	{
		while(stop == false || lockFreeStack.empty() == false)
		{
			if(ewas::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		

			ewas::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	ewas::sleep(100);

	const size_t numEntries = 1000;
	for(size_t index=0;index<numEntries;++index)
	{
		lockFreeStack.push(static_cast<int>(index+1));
	}

	while(lockFreeStack.empty() == false)
	{
		ewas::sleep(100);
	}

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}


	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(EwasLockFreeStackTest,multipleProducerSingleConsumer)
{
	ewas::lock_free_stack<DefaultType> lockFreeStack;
	bool stop1 = false;
	bool stop2 = false;

	auto producerFunc = [&stop1,&lockFreeStack]()
	{
		size_t stackIndex = 0;

		while(stop1 == false)
		{
			lockFreeStack.push(static_cast<int>(++stackIndex));
		}
	};

	auto consumerFunc = [&stop2,&lockFreeStack]()
	{
		while((stop2 == false) || (lockFreeStack.empty() == false))
		{
			if(ewas::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	threadPool[0].start(consumerFunc);

	for(size_t index=1;index<threadPoolSize;++index)
	{
		threadPool[index].start(producerFunc);
	}

	ewas::sleep(1000);

	stop1 = true;

	for(size_t index=1;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop2 = true;

	threadPool[0].stop();

	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(EwasLockFreeStackTest,multipleProducerMultipleConsumer)
{
	ewas::lock_free_stack<DefaultType> lockFreeStack;
	bool stop1 = false;
	bool stop2 = false;

	auto producerFunc = [&stop1,&lockFreeStack]()
	{
		size_t stackIndex = 0;

		while(stop1 == false)
		{
			lockFreeStack.push(static_cast<int>(++stackIndex));
		}
	};
	auto consumerFunc = [&stop2,&lockFreeStack]()
	{
		while(stop2 == false || lockFreeStack.empty() == false)
		{
			if(ewas::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<50;++index)
	{
		threadPool[index].start(producerFunc);
	}
	for(size_t index=50;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	ewas::sleep(5000);

	stop1 = true;

	for(size_t index=0;index<50;++index)
	{
		threadPool[index].stop();
	}

	stop2 = true;

	for(size_t index=50;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	EXPECT_EQ(lockFreeStack.empty(),true);
}
