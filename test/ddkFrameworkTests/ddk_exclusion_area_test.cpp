#include <gtest/gtest.h>
#include "ExclusionArea.h"
#include "ewas_thread.h"
#include <array>

//#define STRESS_TEST

using namespace testing;
using testing::Types;

class EwasExclusionAreaTest : public Test
{
};

TEST(EwasExclusionAreaTest,defaultConstruction)
{
	ExclusionArea exclArea;

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);
}
TEST(EwasExclusionAreaTest,singleReaderNonReentrant)
{
	ExclusionArea exclArea;

	exclArea.enterReader(Reentrancy::NON_REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(EwasExclusionAreaTest,singleWriterNonReentrant)
{
	ExclusionArea exclArea;

	exclArea.enterWriter(Reentrancy::NON_REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(EwasExclusionAreaTest,singleReaderReentrant)
{
	ExclusionArea exclArea;

	exclArea.enterReader(Reentrancy::REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(EwasExclusionAreaTest,singleWriterReentrant)
{
	ExclusionArea exclArea;

	exclArea.enterWriter(Reentrancy::REENTRANT);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(EwasExclusionAreaTest,singleReaderTryToNonReentrant)
{
	ExclusionArea exclArea;

	EXPECT_EQ(exclArea.tryToEnterReader(Reentrancy::NON_REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(EwasExclusionAreaTest,singleWriterTryToNonReentrant)
{
	ExclusionArea exclArea;

	EXPECT_EQ(exclArea.tryToEnterWriter(Reentrancy::NON_REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(EwasExclusionAreaTest,singleReaderTryToReentrant)
{
	ExclusionArea exclArea;

	EXPECT_EQ(exclArea.tryToEnterReader(Reentrancy::REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaverReader();
}
TEST(EwasExclusionAreaTest,singleWriterTryToReentrant)
{
	ExclusionArea exclArea;

	EXPECT_EQ(exclArea.tryToEnterWriter(Reentrancy::REENTRANT),true);

	EXPECT_EQ(exclArea.getReaderState().hasWaitingWriters(), false);

	exclArea.leaveWriter();
}
TEST(EwasExclusionAreaTest,multipleReadersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);
	for(size_t index=0;index<raceDataSize;++index)
	{
		raceData.push_back(std::rand());
	}

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterReader(Reentrancy::NON_REENTRANT);

			const int value = raceData[index];

			EXPECT_EQ(value,raceData[index]);

			exclArea.leaverReader();
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleReadersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);
	for(size_t index=0;index<raceDataSize;++index)
	{
		raceData.push_back(std::rand());
	}

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterReader(Reentrancy::REENTRANT);

			const int value = raceData[index];

			EXPECT_EQ(value,raceData[index]);

			exclArea.leaverReader();

			ewas::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterWriter(Reentrancy::NON_REENTRANT);

			const int randValue = std::rand();

			raceData.push_back(randValue);

			EXPECT_EQ(randValue,raceData.back());

			exclArea.leaveWriter();

			ewas::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData]()
	{
		for(size_t index=0;index<raceDataSize;++index)
		{
			exclArea.enterWriter(Reentrancy::REENTRANT);

			const int randValue = std::rand();

			raceData.push_back(randValue);

			EXPECT_EQ(randValue,raceData.back());

			exclArea.leaveWriter();

			ewas::sleep(10);
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;
	for(size_t index=0;index<100;++index)
	{
		threadPool[index].start(raceFunc);
	}


	for(size_t index=0;index<100;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,singleReaderNonReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::NON_REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	threadPool[0].start(std::bind(raceFunc,true));

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(std::bind(raceFunc,false));
	}


	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(EwasExclusionAreaTest,singleReaderReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	threadPool[0].start(std::bind(raceFunc,true));

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(std::bind(raceFunc,false));
	}

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(EwasExclusionAreaTest,singleReaderReentrantMultipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	threadPool[0].start(std::bind(raceFunc,true));

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].start(std::bind(raceFunc,false));
	}

	for(size_t index=1;index<100;++index)
	{
		threadPool[index].stop();
	}

	stop = true;

	threadPool[0].stop();
}
TEST(EwasExclusionAreaTest,multipleReaderNonReentrantSingleWriterNonReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::NON_REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}

	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(std::bind(raceFunc,true));
	}

	raceFunc(false);

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleReaderReentrantSingleWriterNonReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(std::bind(raceFunc,true));
	}

	raceFunc(false);

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleReaderReentrantSingleWriterReentrant)
{
	static const size_t raceDataSize = 100;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].start(std::bind(raceFunc,true));
	}

	raceFunc(false);

	stop = true;

	for(size_t index=0;index<threadPoolSize;++index)
	{
		threadPool[index].stop();
	}
}
TEST(EwasExclusionAreaTest,multipleReaderNonReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::NON_REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,true));
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,false));
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
TEST(EwasExclusionAreaTest,multipleReaderReentrantMultipleWritersNonReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,true));
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,false));
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
TEST(EwasExclusionAreaTest,multipleReaderReentrantMultipleWritersReentrant)
{
	static const size_t raceDataSize = 1000;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(size_t index=0;index<raceDataSize;++index)
			{
				exclArea.enterWriter(Reentrancy::REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 100;
	std::array<ewas::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<30;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,true));
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,false));
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

TEST(EwasExclusionAreaTest,stressTest)
{
	static const size_t raceDataSize = 1000;
	size_t raceIndex = 0;
	std::vector<int> raceData;
	bool stop = false;

	raceData.reserve(raceDataSize);

	ExclusionArea exclArea;

	auto raceFunc = [&exclArea,&raceData,&raceIndex,&stop](bool i_reader)
	{
		if(i_reader)
		{
			while(stop == false)
			{
				exclArea.enterReader(Reentrancy::NON_REENTRANT);

				if(raceData.empty() == false)
				{
					const int value = raceData.back();

					EXPECT_EQ(value,raceData.back());
				}

				exclArea.leaverReader();

				ewas::sleep(1);
			}
		}
		else
		{
			for(;raceIndex<raceDataSize;++raceIndex)
			{
				exclArea.enterWriter(Reentrancy::NON_REENTRANT);

				const int randValue = std::rand();

				raceData.push_back(randValue);

				EXPECT_EQ(randValue,raceData.back());

				exclArea.leaveWriter();

				std::this_thread::yield();
			}
		}
	};

	static const size_t threadPoolSize = 5000;
	std::array<ewas::thread,threadPoolSize> threadPool;

	size_t threadIndex=0;
	for(;threadIndex<4800;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,true));
	}
	for(;threadIndex<threadPoolSize;++threadIndex)
	{
		threadPool[threadIndex].start(std::bind(raceFunc,false));
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