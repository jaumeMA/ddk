#include <gtest/gtest.h>
#include "ddk_lock_free_stack.h"
#include "test_utils.h"
#include "ddk_thread.h"
#include <array>

using namespace testing;
using testing::Types;

class DDKLockFreeStackTest : public Test
{
};

TEST(DDKLockFreeStackTest,defaultConstruction)
{
	ddk::lock_free_stack<DefaultType> lockFreeStack;

	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(DDKLockFreeStackTest,singleProducerSingleConsumer)
{
	ddk::lock_free_stack<DefaultType> lockFreeStack;

	const size_t numEntries = 1000;
	for(size_t index=0;index<numEntries;++index)
	{
		lockFreeStack.push(static_cast<int>(index));
	}

	EXPECT_EQ(lockFreeStack.empty(),false);

	size_t index = 0;
	while(true)
	{
		if(ddk::optional<DefaultType> currValue = lockFreeStack.pop())
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
TEST(DDKLockFreeStackTest,singleProducerMultipleConsumer)
{
	ddk::lock_free_stack<DefaultType> lockFreeStack;
	bool stop = false;

	auto consumerFunc = [&stop,&lockFreeStack]()
	{
		while(stop == false || lockFreeStack.empty() == false)
		{
			if(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		

			ddk::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	ddk::sleep(100);

	const size_t numEntries = 1000;
	for(size_t index=0;index<numEntries;++index)
	{
		lockFreeStack.push(static_cast<int>(index+1));
	}

	while(lockFreeStack.empty() == false)
	{
		ddk::sleep(100);
	}

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}


	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(DDKLockFreeStackTest,multipleProducerSingleConsumer)
{
	ddk::lock_free_stack<DefaultType> lockFreeStack;
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
			if(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	threadPool[0].start(consumerFunc);

	for(size_t index=1;index<threadPoolSize;++index)
	{
		threadPool[index].start(producerFunc);
	}

	ddk::sleep(1000);

	stop1 = true;

	for(size_t index=1;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop2 = true;

	threadPool[0].stop();

	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(DDKLockFreeStackTest,multipleProducerMultipleConsumer)
{
	ddk::lock_free_stack<DefaultType> lockFreeStack;
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
			if(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}		
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<50;++index)
	{
		threadPool[index].start(producerFunc);
	}
	for(size_t index=50;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	ddk::sleep(5000);

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
