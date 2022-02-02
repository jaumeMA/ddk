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
	ddk::single_consumer_lock_free_stack<DefaultType> lockFreeStack;

	EXPECT_EQ(lockFreeStack.empty(),true);
}
TEST(DDKLockFreeStackTest,singleProducerSingleConsumer)
{
	ddk::single_consumer_lock_free_stack<DefaultType> lockFreeStack;

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
	ddk::multiple_consumer_lock_free_stack<DefaultType> lockFreeStack;
	bool stop = false;

	auto consumerFunc = [&stop,&lockFreeStack]()
	{
		while(stop == false || lockFreeStack.empty() == false)
		{
			if(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				const DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}

			ddk::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	size_t runningThreads = 0;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		if(threadPool[index].start(consumerFunc))
		{
			++runningThreads;
		}
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

	printf("closing\n");

	stop = true;

	for(size_t index=0;index< runningThreads;++index)
	{
		threadPool[index].stop().dismiss();
	}

	printf("checking\n");

	EXPECT_EQ(lockFreeStack.empty(),true);

	printf("leaving\n");
}
TEST(DDKLockFreeStackTest,multipleProducerSingleConsumer)
{
	ddk::single_consumer_lock_free_stack<DefaultType> lockFreeStack;
	bool stop1 = false;
	bool stop2 = false;
	ddk::atomic_size_t itemsProduced = 0;
	ddk::atomic_size_t itemsConsumed = 0;

	auto producerFunc = [&stop1,&lockFreeStack,&itemsProduced]()
	{
		size_t stackIndex = 0;

		while(stop1 == false)
		{
			ddk::atomic_post_increment(itemsProduced);
			lockFreeStack.push(static_cast<int>(++stackIndex));
		}
	};

	auto consumerFunc = [&stop2,&lockFreeStack,&itemsConsumed]()
	{
		while((stop2 == false) || (lockFreeStack.empty() == false))
		{
			if(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				ddk::atomic_post_increment(itemsConsumed);

				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	if(threadPool[0].start(consumerFunc))
	{
		size_t runningThreads = 1;
		for(size_t index=1;index<threadPoolSize;++index)
		{
			if(threadPool[index].start(producerFunc))
			{
				++runningThreads;
			}
		}

		ddk::sleep(1000);

		stop1 = true;

		for(size_t index=1;index< runningThreads;++index)
		{
			threadPool[index].stop().dismiss();
		}

		stop2 = true;

		threadPool[0].stop().dismiss();
	}

	EXPECT_EQ(lockFreeStack.empty(),true);
	EXPECT_EQ(itemsProduced.get(),itemsConsumed.get());
}
TEST(DDKLockFreeStackTest,multipleProducerMultipleConsumer)
{
	ddk::multiple_consumer_lock_free_stack<DefaultType> lockFreeStack;
	bool stop1 = false;
	bool stop2 = false;

	auto producerFunc = [&stop1,&lockFreeStack]()
	{
		size_t stackIndex = 0;

		while(stop1 == false)
		{
			lockFreeStack.push(static_cast<int>(++stackIndex));

			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	};
	auto consumerFunc = [&stop2,&lockFreeStack]()
	{
		while(stop2 == false || lockFreeStack.empty() == false)
		{
			while(ddk::optional<DefaultType> currValueOpt = lockFreeStack.pop())
			{
				DefaultType currValue = *currValueOpt;

				EXPECT_GT(currValue,0);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(33));
		}
	};

	const size_t threadPoolSize = 500;
	const size_t numProducerThreads = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;
	size_t runningProducerThreads = 0;

	for(size_t index=0;index< numProducerThreads;++index)
	{
		if(threadPool[index].start(producerFunc))
		{
			++runningProducerThreads;
		}
	}

	size_t runningConsumerThreads = numProducerThreads;
	for(size_t index= numProducerThreads;index<threadPoolSize;++index)
	{
		if(threadPool[index].start(consumerFunc))
		{
			++runningConsumerThreads;
		}
	}

	ddk::sleep(15000);

	stop1 = true;

	for(size_t index=0;index< runningProducerThreads;++index)
	{
		threadPool[index].stop().dismiss();
	}

	stop2 = true;

	for(size_t index= numProducerThreads;index< runningConsumerThreads;++index)
	{
		threadPool[index].stop().dismiss();
	}

	EXPECT_EQ(lockFreeStack.empty(),true);
}
