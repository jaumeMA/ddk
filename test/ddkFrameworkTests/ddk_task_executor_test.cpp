#include <gtest/gtest.h>
#include "ddk_task_executor.h"
#include "ddk_atomics.h"

using namespace testing;

class DDKTaskExecutorTest: public Test
{
};

TEST(DDKTaskExecutorTest,stressTest)
{
    ddk::atomic_size_t numExecutedTasks = 0;
    ddk::atomic_size_t numCancelledTasks = 0;
    ddk::task_executor taskExecutor(30,100);

	if(taskExecutor.start())
    {
        printf("done\n");

        const size_t k_numProducers = 10;
        const size_t k_numTasksPerProducer = 1000;
        ddk::thread producers[k_numProducers];
        for(size_t producerId=0;producerId<k_numProducers;++producerId)
        {
            producers[producerId].start(ddk::make_function([&]()
            {
                for(size_t taskIndex=0;taskIndex< k_numTasksPerProducer;++taskIndex)
                {
                    taskExecutor.enqueue([&]()
                    {
                        ddk::atomic_post_increment(numExecutedTasks);
                        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        return 10;
                    });

                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            })).dismiss();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        for(size_t producerId = 0; producerId < k_numProducers; ++producerId)
        {
            producers[producerId].stop().dismiss();
        }

        taskExecutor.stop();
        printf("done\n");
    }
	//EXPECT_EQ(numExecutedTasks.get(),k_numProducers * k_numTasksPerProducer);

}
