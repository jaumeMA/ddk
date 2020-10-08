#include <gtest/gtest.h>
#include "ddk_iterable.h"
#include <vector>

using namespace testing;

class DDKIterableTest : public Test
{
};

struct A
{
};
struct B : A
{
};
struct C : A
{
};
struct D : C
{
};
struct E : C
{
};

TEST(DDKIterableTest, forwardIterableConstruction)
{
    std::vector<int> foo;
    foo.push_back(10);
    foo.push_back(-25);
    foo.push_back(4);
    foo.push_back(1897);
    foo.push_back(76);


    ddk::const_random_access_iterable<int> fooIterable = ddk::make_iterable<ddk::random_access_iterable<int>>(foo);

    ddk::const_random_access_iterable<const int> transformedFoo = ddk::make_function([](int i_value) { return 2 * i_value; }) <<= fooIterable;

    ddk::make_function([](ddk::const_bidirectional_value<const int> i_value){ printf("current value: %d\n",*i_value); }) <<=  ddk::make_function([](int i_value) { return 2 * i_value; }) <<= fooIterable;
}
TEST(DDKIterableTest, iterableUnion)
{
    std::vector<A> foo1;
    std::vector<D> foo2;
    std::vector<E> foo3;

    ddk::const_random_access_iterable<A> fooIterable1 = ddk::make_iterable<ddk::random_access_iterable<A>>(foo1);
    ddk::const_random_access_iterable<D> fooIterable2 = ddk::make_iterable<ddk::random_access_iterable<D>>(foo2);
    ddk::const_random_access_iterable<E> fooIterable3 = ddk::make_iterable<ddk::random_access_iterable<E>>(foo3);

    ddk::const_random_access_iterable<A> fooIterableUnion1 = fooIterable1 | fooIterable2;
//    ddk::const_random_access_iterable<A> fooIterableUnion2 = fooIterable1 | fooIterable2 | fooIterable3;
}
