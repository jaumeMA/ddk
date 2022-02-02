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
    ddk::task_executor taskExecutor(30,45);

	if(taskExecutor.start())
    {
        printf("done\n");

        const size_t k_numProducers = 10;
        const size_t k_numTasksPerProducer = 2000;
        ddk::thread producers[k_numProducers];
        for(size_t producerId=0;producerId<k_numProducers;++producerId)
        {
            producers[producerId].start(ddk::make_function([&]()
            {
                for(size_t taskIndex=0;taskIndex< k_numTasksPerProducer;++taskIndex)
                {
                    taskExecutor.enqueue(ddk::make_function([&]()
                    {
                        ddk::atomic_post_increment(numExecutedTasks);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
                        return 10;
                    }))
                    .on_error(ddk::make_function([&numCancelledTasks](const ddk::async_error& i_error)
                    {
                        ddk::atomic_post_increment(numCancelledTasks);
                    }))
                    .then(ddk::make_function([](int i_value) -> char
                    {
                        return 'a';
                    }));

                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            })).dismiss();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for(size_t producerId = 0; producerId < k_numProducers; ++producerId)
        {
            producers[producerId].stop().dismiss();
        }

        taskExecutor.stop();
        printf("done\n");
    }
	//EXPECT_EQ(numExecutedTasks.get(),k_numProducers * k_numTasksPerProducer);

}
