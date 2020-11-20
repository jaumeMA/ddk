#include "ddk_iterable.h"
#include <gtest/gtest.h>
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

	void prova(const ddk::function<void()>&, const std::string&,ddk::lent_reference_wrapper<A>)
	{
	}
	int operator*() const
    {
        return m_value;
    }
	bool operator==(int i_value) const
	{
		return m_value == i_value;
	}
	bool operator>(int i_value) const
	{
		return m_value > i_value;
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

void lololo()
{
}

class foo_interface
{
public:
	virtual ~foo_interface() = default;

	virtual int hola() const = 0;
	virtual int adeu() const = 0;
};

template<typename ... Iterables>
class foo : public foo_interface
{
public:
foo(const Iterables& ... i_iterables)
: m_foo(i_iterables...)
{
}
int hola() const
{
	return 0;
}
int adeu() const
{
	return 1;
}

private:
	int a = 0;
	ddk::tuple<Iterables...> m_foo;
};

TEST(DDKIterableTest, forwardIterableConstruction)
{
	std::vector<int> foo;
    foo.push_back(10);
    foo.push_back(4);
    foo.push_back(-25);
    foo.push_back(1897);
    foo.push_back(76);

    ddk::const_random_access_iterable<int> fooIterable = ddk::make_iterable<ddk::random_access_iterable<int>>(foo);

	ddk::make_function([](ddk::const_bidirectional_value<int> i_value) { printf("current value: %d at %zd\n",*i_value,value_position(i_value)); }) <<= fooIterable;
	
	ddk::const_random_access_iterable<const int> transformedFoo = ddk::view::transform([](int i_value) { return 2 * i_value; }) <<= fooIterable;

	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("current value: %d at %zd\n", *i_value, value_position(i_value)); }) <<= ddk::view::order(ddk::iter::reverse_order) <<= ddk::view::filter([](const A& i_value) { return i_value > 0; }) <<= ddk::view::transform([](int i_value) { return A(i_value); }) <<= fooIterable;
}
TEST(DDKIterableTest, iterableUnion)
{
	char _[4096] = { 0 };

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

    const ddk::const_random_access_iterable<A> fooIterable1 = ddk::make_iterable<ddk::random_access_iterable<A>>(foo1);
    const ddk::const_random_access_iterable<D> fooIterable2 = ddk::make_iterable<ddk::random_access_iterable<D>>(foo2);
    const ddk::const_random_access_iterable<E> fooIterable3 = ddk::make_iterable<ddk::random_access_iterable<E>>(foo3);

	const size_t size1 = sizeof(ddk::const_random_access_iterable<A>);
	const size_t size2 = sizeof(ddk::const_random_access_iterable<D>);
	const size_t align1 = std::alignment_of<ddk::const_random_access_iterable<A>>::value;
	const size_t align2 = std::alignment_of<ddk::const_random_access_iterable<D>>::value;

	ddk::tuple<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> fooIterable4(fooIterable1,fooIterable2);
	foo<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> fooIterable(fooIterable1,fooIterable2);
	ddk::detail::union_iterable_impl<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> unionIterable(fooIterable1,fooIterable2);

	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("current value: %d at %zd\n",**i_value,value_position(i_value)); }) <<=  (fooIterable1 | fooIterable2);
    ddk::const_random_access_iterable<A> fooIterableUnion2 = fooIterable1 | fooIterable2 | fooIterable3;
    ddk::make_function([](ddk::const_bidirectional_value<const A, const D,const A> i_value){ printf("current values tuple: %d, %d, %d\n",*(i_value->get<0>()),*(i_value->get<1>()),*(i_value->get<2>())); }) <<= ddk::view::order(ddk::iter::reverse_order) <<= ((fooIterable1 | fooIterable2) & fooIterable2 & (fooIterable1 | fooIterable3)), ddk::make_function([](ddk::iter::action_result i_result){ if(i_result != ddk::success) printf("error: %d\n", i_result.error().get_nested_error<ddk::iter::EraseActionError>().getValue()); });
}
