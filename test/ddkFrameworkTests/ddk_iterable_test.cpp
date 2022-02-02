#include "ddk_iterable.h"
#include <gtest/gtest.h>
#include <vector>
#include "ddk_iterable.h"
#include "ddk_high_order_array.h"
#include "ddk_tuple.h"
#include "ddk_one_to_n_action_adapter.h"

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
	std::map<int,ddk::unique_reference_wrapper<int>> kk;
	std::map<int,int> _foo;
	_foo.insert(std::make_pair(1,2));
	_foo.insert(std::make_pair(2,2));
	_foo.insert(std::make_pair(3,2));
	_foo.insert(std::make_pair(4,2));
	_foo.insert(std::make_pair(5,2));
	std::vector<int> foo;
    foo.push_back(1);
    foo.push_back(2);
    foo.push_back(3);
    foo.push_back(4);
    foo.push_back(5);

	ddk::unique_reference_wrapper cucu = ddk::make_unique_reference<int>(10);
	ddk::lent_reference_wrapper<int> kkk = ddk::lend(cucu);
	ddk::lent_reference_wrapper<const int> kkkk(kkk);

	ddk::const_bidirectional_iterable<const ddk::lent_reference_wrapper<const int>> myIterable = ddk::iter::transform([](const std::pair<const int,const ddk::unique_reference_wrapper<int>&>& i_pair) -> ddk::lent_reference_wrapper<const int> { return ddk::lend(i_pair.second); }) <<= ddk::view::filter([](const std::pair<const int,const ddk::unique_reference_wrapper<int>&>& i_pair){ return i_pair.first > 0; }) <<= kk;

	ddk::make_function([](const std::pair<const int,int>& i_value1) 
	{ 
		printf("value: %d\n",i_value1.second); 
	}) <<= _foo;

	ddk::make_function([](int i_value1)
	{
		printf("value: %d\n",i_value1);
	}) <<= foo;

	//ddk::combine<ddk::iter::one_to_n_diagonal_action_adapter>(foo,foo);
	//ddk::combine(ddk::iter::one_to_n_enumerate_action_adapter{foo,foo},foo,foo);

	ddk::high_order_array<int,2,2> _highOrderProva = {1,1,2,2};
	ddk::high_order_array<size_t,2,2> highOrderProva;

	highOrderProva[0][0] = 1;
	highOrderProva[0][1] = 2;
	highOrderProva[1][0] = 3;
	highOrderProva[1][1] = 4;

	ddk::const_random_access_iterable<size_t> highOrderIterable = ddk::make_iterable<ddk::const_random_access_iterable<size_t>>(highOrderProva);

	ddk::make_function([](size_t i_value) {printf("cur integer: %d\n",i_value); }) <<= ddk::view::take_n(ddk::odd_integers,10);

	ddk::make_function([](size_t i_value){printf("cur high order value: %d\n",i_value);}) <<= highOrderIterable;
	ddk::make_function([](size_t i_value) {printf("cur high order alternate value: %d\n",i_value); }) <<= ddk::view::order(ddk::alternate_order(4)) <<= highOrderIterable;
	ddk::make_function([](size_t i_value) {printf("cur high transpose dimension value: %d\n",i_value); }) <<= ddk::view::order(ddk::transponse_dimension_order) <<= highOrderProva;

	std::vector<size_t> highOrderProvaSuma;
	//const auto res = ddk::iter::sum <<= ddk::iter::pow(ddk::arg_0,2.f) <<= ddk::iter::sum(ddk::iter::transform([](int i_value) { return 2.f * i_value; }) <<= foo,foo,foo);
	highOrderProva <<= ddk::iter::pow <<= ddk::fusion(foo,foo);
	highOrderProvaSuma <<= ddk::iter::inv(foo);
	int provaSuma = ddk::iter::sum(foo);
	int provaSuma2 = ddk::sum({10,4,-25,1897,76});

	ddk::tuple<double,int,float> provaTuple;
	provaTuple.set<0>(10.2f);
	provaTuple.set<1>(20);
	provaTuple.set<2>(30.5f);

	ddk::make_function([](size_t i_value){}) <<= highOrderProvaSuma;

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

	[](int i_value)
	{
		printf("valor trobat: %d\n",i_value);
	} <<= ddk::iter::find_if(foo,[](const int& i_value){ return i_value > 1; });

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
	foo2.push_back(-67);

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
		ddk::make_function([](const A& i_value) { printf("current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= iterable;
		printf("done\n");
	}

	ddk::make_function([](const A& i_value1,const A& i_value2){ printf("VALORS: %d, %d\n",*i_value1,*i_value2);}) <<= ddk::fusion(fooIterable1,fooIterable2);

	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("1 current value: %d at %zd\n",**i_value,value_position(i_value)); }) <<=  ddk::concat(fooIterable1,fooIterable2);
    ddk::const_random_access_iterable<A> fooIterableUnion2 = ddk::concat(fooIterable1,fooIterable2,fooIterable3);
	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("2 current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= fooIterableUnion2,ddk::make_function([](ddk::action_result i_result) { if(i_result != ddk::success) printf("error: %d\n",i_result.error().get_nested_error<ddk::EraseActionError>().getValue()); });
	ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("3 current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= ddk::iter::transform([](const A& i_val1, const D& i_val2, const A& i_val3) { return i_val1; }) <<= ddk::fusion(ddk::concat(fooIterable1,fooIterable2),fooIterable2, ddk::concat(fooIterable1,fooIterable3)), ddk::make_function([](ddk::action_result i_result){ if(i_result != ddk::success) printf("error: %d\n", i_result.error().get_nested_error<ddk::EraseActionError>().getValue()); });
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
