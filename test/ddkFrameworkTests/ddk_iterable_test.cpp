#include <gtest/gtest.h>
#include "ddk_iterable.h"
#include <vector>

using namespace testing;

class DDKIterableTest : public Test
{
};

struct A
{
public:
    A(int i_value)
    : m_value(i_value)
    {
    }

    int operator*() const
    {
        return m_value;
    }

private:
    int m_value;
};
struct B : A
{
    using A::A;
};
struct C : A
{
    using A::A;
};
struct D : C
{
    using C::C;
};
struct E : C
{
    using C::C;
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

    ddk::make_function([](ddk::const_bidirectional_value<const int> i_value){ printf("current value: %d at %d\n",*i_value,value_position(i_value)); }) <<=  ddk::make_function([](int i_value) { return 2 * i_value; }) <<= fooIterable;
}
TEST(DDKIterableTest, iterableUnion)
{
    std::vector<A> foo1;
    foo1.push_back(10);
    foo1.push_back(-25);
    foo1.push_back(4);
    foo1.push_back(1897);
    foo1.push_back(76);

    std::vector<D> foo2;
    foo2.push_back(88);
    foo2.push_back(-347);
    foo2.push_back(101);
    foo2.push_back(3);
    foo2.push_back(-53);

    std::vector<E> foo3;
    foo3.push_back(66);
    foo3.push_back(167);
    foo3.push_back(7639);
    foo3.push_back(-46);
    foo3.push_back(-189);

    ddk::const_random_access_iterable<A> fooIterable1 = ddk::make_iterable<ddk::random_access_iterable<A>>(foo1);
    ddk::const_random_access_iterable<D> fooIterable2 = ddk::make_iterable<ddk::random_access_iterable<D>>(foo2);
    ddk::const_random_access_iterable<E> fooIterable3 = ddk::make_iterable<ddk::random_access_iterable<E>>(foo3);

    ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("current value: %d at %d\n",**i_value,value_position(i_value)); }) <<=  (fooIterable1 | fooIterable2);
    ddk::const_random_access_iterable<A> fooIterableUnion2 = fooIterable1 | fooIterable2 | fooIterable3;
    ddk::make_function([](ddk::const_bidirectional_value<const A,const D,const A> i_value){ printf("current values tuple: %d, %d, %d\n",*i_value->get<0>(),*i_value->get<1>(),*i_value->get<2>()); }) <<=  (fooIterable1 & fooIterable2 & fooIterable3);
}
