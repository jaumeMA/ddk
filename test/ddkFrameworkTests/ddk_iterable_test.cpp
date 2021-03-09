#include "ddk_iterable.h"
#include <gtest/gtest.h>
#include <vector>
#include "ddk_iterable_algorithm.h"
#include "ddk_high_order_array.h"
#include "ddk_tuple_adaptor.h"

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

namespace ddk
{

struct tuple_visitor
{
    struct callable_tag;

	template<typename T>
	void operator()(T&& i_value) const
	{
		printf("A loperator de tuples");
	}
};

}

TEST(DDKIterableTest, forwardIterableConstruction)
{
	std::vector<int> foo;
    foo.push_back(10);
    foo.push_back(4);
    foo.push_back(-25);
    foo.push_back(1897);
    foo.push_back(76);

	ddk::high_order_array<size_t,2,2> highOrderProva;

	highOrderProva[0][0] = 1;
	highOrderProva[0][1] = 2;
	highOrderProva[1][0] = 3;
	highOrderProva[1][1] = 4;

	ddk::const_random_access_iterable<size_t> highOrderIterable = ddk::make_iterable<ddk::const_random_access_iterable<size_t>>(highOrderProva);

	ddk::make_function([](size_t i_value){printf("cur high order value: %d\n",i_value);}) <<= highOrderIterable;

	std::vector<size_t> highOrderProvaSuma;
	const auto res = ddk::iter::sqrt <<= ddk::iter::sum(ddk::iter::transform([](int i_value) { return 2.f * i_value; }) <<= foo,foo,foo);
	highOrderProvaSuma <<= ddk::iter::inv(foo);
	int provaSuma = ddk::iter::sum(foo);
	int provaSuma2 = ddk::sum({10,4,-25,1897,76});

	ddk::tuple<double,int,float> provaTuple;
	provaTuple.set<0>(10.2f);
	provaTuple.set<1>(20);
	provaTuple.set<2>(30.5f);

	ddk::const_random_access_iterable<ddk::const_variant_const_reference<double,int,float>> provaTuplaIterable = ddk::make_iterable<ddk::const_random_access_iterable<ddk::const_variant_const_reference<double,int,float>>>(provaTuple);

	ddk::tuple<double,int,float> provaSumTuple;

	//provaSumTuple <<= ddk::iter::sum(provaTuplaIterable,provaTuplaIterable,provaTuplaIterable);

	ddk::make_function([](const ddk::variant_const_reference<double,int,float>& i_value)
	{
		if(i_value.is<const double&>())
		{
			printf("curr tupla str value: %f\n",i_value.get<const double&>());
		}
		else if(i_value.is<const int&>())
		{
			printf("curr tupla int value: %d\n",i_value.get<const int&>());
		}
		else
		{
			printf("curr tupla float value: %f\n",i_value.get<const float&>());
		}

	}) <<= provaTuplaIterable;

	ddk::tuple_visitor tupleVisitor;

	tupleVisitor <<= provaTuple;

	ddk::const_random_access_iterable<int> fooIterable = ddk::make_iterable<ddk::random_access_iterable<int>>(foo);

	auto deducedIterable = ddk::deduce_iterable(foo);
	auto deducedFooIterable = ddk::deduce_iterable(fooIterable);

	ddk::thread myThread;
	((ddk::make_function([](ddk::const_bidirectional_value<int> i_value) { printf("1 current value: %d at %zd\n",*i_value,value_position(i_value)); }) <<= fooIterable) -> attach(std::move(myThread))).then(ddk::make_function([](const ddk::action_result&){}));

	//ddk::algo::swap swaper(ddk::make_function([](int i_value){ return i_value == 4; }),ddk::make_function([](int i_value){ return i_value == 76; }));

	ddk::const_random_access_iterable<int> transformedFoo = ddk::view::filter([](const int& i_value) { return i_value > 0; }) <<= foo;

	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("2 current value: %d at %zd\n", *i_value, value_position(i_value)); }) <<= ddk::view::order(ddk::reverse_order) <<= ddk::view::filter([](const A& i_value) { return i_value > 0; }) <<= ddk::iter::transform([](int i_value) { return A(i_value); }) <<= fooIterable;

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

	ddk::tuple<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> fooIterable4(fooIterable1,fooIterable2);
	ddk::detail::union_iterable_impl<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> unionIterable(fooIterable1,fooIterable2);

	for(const auto& iterable : ddk::view::group_by(ddk::concat(fooIterable1,fooIterable2,fooIterable3),[](const A& i_value){ return *i_value > 100; },[](const A& i_value){ return *i_value < 50; }))
	{
		ddk::make_function([](const A& i_value) { printf("current value: %d\n",*i_value); }) <<= iterable;
		printf("done\n");
	}

	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("1 current value: %d at %zd\n",**i_value,value_position(i_value)); }) <<=  ddk::concat(fooIterable1,fooIterable2);
    ddk::const_random_access_iterable<A> fooIterableUnion2 = ddk::concat(fooIterable1,fooIterable2,fooIterable3);
	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("2 current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= fooIterableUnion2,ddk::make_function([](ddk::action_result i_result) { if(i_result != ddk::success) printf("error: %d\n",i_result.error().get_nested_error<ddk::EraseActionError>().getValue()); });
	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("3 current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= ddk::iter::transform([](ddk::values_tuple<const A,const D,const A> i_value) { return i_value->get<0>(); }) <<= ddk::fusion(ddk::concat(fooIterable1,fooIterable2),fooIterable2, ddk::concat(fooIterable1,fooIterable3)), ddk::make_function([](ddk::action_result i_result){ if(i_result != ddk::success) printf("error: %d\n", i_result.error().get_nested_error<ddk::EraseActionError>().getValue()); });
}

std::vector<int> createLargeVector2(size_t size)
{
	std::vector<int> res;

	for(size_t index = 0; index < size; index++)
	{
		res.push_back(index);
	}

	return std::move(res);
}

std::vector<int> myLargeVector2 = createLargeVector2(1000000);

TEST(DDKIterableTest,directIteration)
{
	ddk::make_function([](int i_value) {}) <<= myLargeVector2;
}
