#include <gtest/gtest.h>
#include "ddk_exclusion_area.h"
#include "ddk_thread.h"
#include <array>

//#define STRESS_TEST

using namespace testing;
using testing::Types;

class DDKExclusionAreaTest : public Test
{
};

TEST(DDKExclusionAreaTest,defaultConstruction)
{
	ddk::exclusion_area exclArea;

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);
}
TEST(DDKExclusionAreaTest,singleReaderNonReentrant)
{
	ddk::exclusion_area exclArea;

	exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(DDKExclusionAreaTest,singleWriterNonReentrant)
{
	ddk::exclusion_area exclArea;

	exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(DDKExclusionAreaTest,singleReaderReentrant)
{
	ddk::exclusion_area exclArea;

	exclArea.enterReader(ddk::Reentrancy::REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(DDKExclusionAreaTest,singleWriterReentrant)
{
	ddk::exclusion_area exclArea;

	exclArea.enterWriter(ddk::Reentrancy::REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(DDKExclusionAreaTest,singleReaderTryToNonReentrant)
{
	ddk::exclusion_area exclArea;

	EXPECT_EQ(exclArea.tryToEnterReader(ddk::Reentrancy::NON_REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(DDKExclusionAreaTest,singleWriterTryToNonReentrant)
{
	ddk::exclusion_area exclArea;

	EXPECT_EQ(exclArea.tryToEnterWriter(ddk::Reentrancy::NON_REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(DDKExclusionAreaTest,singleReaderTryToReentrant)
{
	ddk::exclusion_area exclArea;

	EXPECT_EQ(exclArea.tryToEnterReader(ddk::Reentrancy::REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(DDKExclusionAreaTest,singleWriterTryToReentrant)
{
	ddk::exclusion_area exclArea;

	EXPECT_EQ(exclArea.tryToEnterWriter(ddk::Reentrancy::REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(DDKExclusionAreaTest,multipleReadersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);
	for(size_t index=0;index<raceDataSize;++index)
	{
		raceData.push_back(std::rand());
	}

	ddk::exclusion_area exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

			const int value = raceData[index];

			EXPECT_EQ(value,raceData[index]);

			exclArea.leaverReader();
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReadersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);
	for(size_t index=0;index<raceDataSize;++index)
	{
		raceData.push_back(std::rand());
	}

	ddk::exclusion_area exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterReader(ddk::Reentrancy::REENTRANT);

			const int value = raceData[index];

			EXPECT_EQ(value,raceData[index]);

			exclArea.leaverReader();

			ddk::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

			const int randValue = std::rand();

			raceData.push_back(randValue);

			EXPECT_EQ(randValue,raceData.back());

			exclArea.leaveWriter();

			ddk::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterWriter(ddk::Reentrancy::REENTRANT);

			const int randValue = std::rand();

			raceData.push_back(randValue);

			EXPECT_EQ(randValue,raceData.back());

			exclArea.leaveWriter();

			ddk::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,singleReaderNonReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
    bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
    auto producerFunc = [&exclArea,&raceData]()
    {
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
    };


	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	threadPool[0].start(consumerFunc);
	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(producerFunc);
	}

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(DDKExclusionAreaTest,singleReaderReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
    {
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
    };

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	threadPool[0].start(consumerFunc);

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(producerFunc);
	}

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(DDKExclusionAreaTest,singleReaderReentrantMultipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
    {
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
    };

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	threadPool[0].start(consumerFunc);

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(producerFunc);
	}

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(DDKExclusionAreaTest,multipleReaderNonReentrantSingleWriterNonReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
    };
	auto producerFunc = [&exclArea,&raceData]()
	{
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	producerFunc();

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReaderReentrantSingleWriterNonReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
	{
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	producerFunc();

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReaderReentrantSingleWriterReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
    {
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
    };

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(consumerFunc);
	}

	producerFunc();

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReaderNonReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
    {
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
    };

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(consumerFunc);
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(producerFunc);
	}

	for(size_t index=30;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	for(size_t index=0;index<30;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReaderReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
	{
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(consumerFunc);
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(producerFunc);
	}

	for(size_t index=30;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	for(size_t index=0;index<30;++index)
	{
		threadPool[index].stop();
	}
}
TEST(DDKExclusionAreaTest,multipleReaderReentrantMultipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData]()
	{
        for(size_t index=0;index<raceDataSize;++index)
        {
            exclArea.enterWriter(ddk::Reentrancy::REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
	};

	static const size_t threadPoolSize = 100;
	std::array<ddk::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(consumerFunc);
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(producerFunc);
	}

	for(size_t index=30;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	for(size_t index=0;index<30;++index)
	{
		threadPool[index].stop();
	}
}

#if defined(STRESS_TEST)

TEST(DDKExclusionAreaTest,stressTest)
{
	static const size_t raceDataSize = 1000;
	size_t raceIndex = 0;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ddk::exclusion_area exclArea;

	auto consumerFunc = [&exclArea,&raceData,&raceIndex,&stop]()
	{
        while(stop == false)
        {
            exclArea.enterReader(ddk::Reentrancy::NON_REENTRANT);

            if(raceData.empty() == false)
            {
                const int value = raceData.back();

                EXPECT_EQ(value,raceData.back());
            }

            exclArea.leaverReader();

            ddk::sleep(1);
        }
	};
	auto producerFunc = [&exclArea,&raceData,&raceIndex,&stop]()
	{
        for(;raceIndex<raceDataSize;++raceIndex)
        {
            exclArea.enterWriter(ddk::Reentrancy::NON_REENTRANT);

            const int randValue = std::rand();

            raceData.push_back(randValue);

            EXPECT_EQ(randValue,raceData.back());

            exclArea.leaveWriter();

            std::this_thread::yield();
        }
	};

	static const size_t threadPoolSize = 5000;
	std::array<ddk::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<4800;++threadIndex)
	{
		threadPool[threadIndex].start(consumerFunc);
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(producerFunc);
	}

	for(size_t index=4800;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	for(size_t index=0;index<4800;++index)
	{
		threadPool[index].stop();
	}
}

#endif
