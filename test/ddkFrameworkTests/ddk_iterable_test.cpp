#include <gtest/gtest.h>
#include "ddk_iterable.h"
#include <vector>

using namespace testing;

class DDKIterableTest : public Test
{
};

TEST(DDKIterableTest, forwardIterableConstruction)
{
    std::vector<int> foo;
    ddk::thread myThread;

    ddk::const_random_access_iterable<int> fooIterable = ddk::make_iterable<ddk::random_access_iterable<int>>(foo);

    ddk::const_random_access_iterable<const int> transformedFoo = ddk::make_filter_function([](const int& i_value) -> bool { return i_value%4 == 0;}) <<= ddk::make_function([](int i_value) { return 2 * i_value; }) <<= fooIterable;

    ddk::future<void> itFuture = attach(std::move(myThread)) <<= ddk::make_function([](ddk::const_random_accessed_value<const int> i_value){ printf("current value: %d\n",*i_value); i_value = prev_value(std::move(i_value)); }) <<= fooIterable;
}
