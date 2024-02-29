#include <gtest/gtest.h>
#include <vector>
#include <map>
#include "ddk_unique_pointer_wrapper.h"
#include "ddk_iterable.h"
//#include "ddk_high_order_array.h"
//#include "ddk_tuple.h"
//#include "ddk_tuple_adaptor.h"
//#include "ddk_one_to_n_action_adapter.h"

using namespace testing;

class DDKIterableTest : public Test
{
};

struct A
{
public:
    A(int i_value = 0)
    : m_value(i_value)
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
	bool operator>(const A& other) const
	{
		return m_value > other.m_value;
	}
	A operator+(const A& other) const
	{
		return m_value + other.m_value;
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
	typedef void return_type;

	template<typename T>
	void operator()(T&& i_value) const
	{
		printf("A loperator de tuples");
	}
};

}

static std::list<int> myFoo;
void proveta2(ddk::const_bidirectional_iterable<const int> i_iterable)
{
	[](const int& i_value)
	{
		int a = 0;
		++a;
	}   <<= i_iterable;
}

auto proveta()
{
	myFoo.push_back(1);
	myFoo.push_back(2);
	myFoo.push_back(3);
	myFoo.push_back(-4);
	myFoo.push_back(5);

	return ddk::view::order(ddk::reverse_order)
			<<= ddk::deduce_iterable(myFoo);
}

std::vector<int> create_prova()
{
	std::vector<int> kk;

	kk.reserve(1000000);

	for (size_t i = 0;i < 1000000;i++)
	{
		kk.push_back(i);
	}

	return kk;
}

std::vector<int> v_prova = create_prova();

struct myBeginAction : ddk::action_base
{
	typedef ddk::mpl::type_pack<ddk::begin_action_tag> tags_t;

	myBeginAction(bool i_valid = true)
		: action_base(i_valid)
	{
	}
	TEMPLATE(typename Adaptor)
		REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
		inline ddk::no_action apply(Adaptor&& i_adaptor) const
	{
		i_adaptor.perform_action(ddk::begin_action_tag{}).dismiss();

		return false;
	}
};
template<typename Sink>
struct myForwardAction : ddk::action_base
{
	typedef ddk::mpl::type_pack<ddk::forward_action_tag> tags_t;

	myForwardAction(const Sink& i_sink, bool i_valid = true)
	: action_base(i_valid)
	, m_action(i_valid)
	, m_sink(i_sink)
	{
	}
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	inline myForwardAction apply(Adaptor&& i_adaptor) const
	{
		if (m_action.apply(i_adaptor))
		{
			i_adaptor.perform_action(ddk::sink_action_tag{ m_sink }).dismiss();

			return { m_sink,true };
		}
		else
		{
			return { m_sink,false };
		}
	}

	ddk::forward_action m_action;
	Sink m_sink;
};
template<typename Sink>
myForwardAction(const Sink&,bool i_valid) -> myForwardAction<Sink>;

TEST(DDKIterableTest,peformance)
{
	ddk::go_to_begin >>= ddk::concat(v_prova,v_prova);
}
struct myAdaptor
{
	typedef ddk::detail::by_value_traits<ddk::unique_reference_wrapper<int>> traits;
	typedef traits const_traits;
};

TEST(DDKIterableTest,forwardIterableConstruction)
{
	std::map<int,ddk::unique_reference_wrapper<int>> kk;
	std::map<int,int> _foo;
	_foo.insert(std::make_pair(1,1));
	_foo.insert(std::make_pair(2,2));
	_foo.insert(std::make_pair(3,3));
	_foo.insert(std::make_pair(4,4));
	_foo.insert(std::make_pair(5,5));
	std::vector<int> foo;
	foo.push_back(1);
	foo.push_back(2);

	foo.push_back(3);
	foo.push_back(-4);
	foo.push_back(-5);

	auto cucu = ddk::view::take_n(ddk::deduce_iterable(foo),10);

	//ddk::swap_action{} >>= ddk::iter::transform([](std::pair<const int,int>& i_value) -> int& { return i_value.second; }) <<= ddk::view::filter([](const std::pair<const int,int>& i_value) { return i_value.second > 0; }) <<= _foo;

	[](auto&&)
	{
	} <<= ddk::view::filter([](const std::pair<const int,ddk::unique_reference_wrapper<int>>&) { return true; }) <<= kk;

	ddk::const_bidirectional_value_iterable<const int> _ = ddk::iter::transform([](std::pair<const int,int>& i_value) -> int& { return i_value.second; }) <<= ddk::view::filter([](const std::pair<const int,int>& i_value) { return i_value.second > 0; }) <<= _foo;

	[](const int&)
	{

	} <<= _;

	proveta2(proveta());

	//ddk::unique_reference_wrapper cucu = ddk::make_unique_reference<int>(10);
	//ddk::lent_reference_wrapper<int> kkk = ddk::lend(cucu);
	//ddk::lent_reference_wrapper<const int> kkkk(kkk);

	////ddk::const_bidirectional_iterable<const ddk::lent_reference_wrapper<const int>> myIterable = ddk::iter::transform([](const std::pair<const int,const ddk::unique_reference_wrapper<int>&>& i_pair) -> ddk::lent_reference_wrapper<const int> { return ddk::lend(i_pair.second); }) <<= ddk::view::filter([](const std::pair<const int,const ddk::unique_reference_wrapper<int>&>& i_pair){ return i_pair.first > 0; }) <<= kk;

	////ddk::task_executor taskExecutor(10,10);

	//[](const std::pair<const int,int>& i_value1)
	//{
	//	printf("value: %d\n",i_value1.second);
	//} <<= _foo;

	//[](int i_value1)
	//{
	//	printf("value: %d\n",i_value1);
	//} <<= foo;

	//////ddk::combine<ddk::iter::one_to_n_diagonal_action_adapter>(foo,foo);
	//////ddk::combine(ddk::iter::one_to_n_enumerate_action_adapter{foo,foo},foo,foo);

	//ddk::high_order_array<int,2,2> _highOrderProva = {1,1,2,2};
	ddk::high_order_array<int,2,2> highOrderProva;

	highOrderProva[0][0] = 1;
	highOrderProva[0][1] = 2;
	highOrderProva[1][0] = 3;
	highOrderProva[1][1] = 4;

	////ddk::const_random_access_iterable<int> highOrderIterable = ddk::make_iterable<ddk::const_random_access_iterable<int>>(highOrderProva);

	[](const int& i_value) 
	{
		printf("cur integer: %d\n",i_value);
	} <<= highOrderProva;

	[](const int& i_value)
	{
		printf("cur integer: %d\n",i_value);
	}	<<= ddk::view::filter([](const int& i_value) { return i_value > 0; })
		<<= ddk::view::order(ddk::transponse_dimension_order)
		<<= highOrderProva;

	////auto highOrderReceiver = ddk::make_function([](const int& i_value)
	////{
	////	printf("cur high order value: %d\n",i_value);
	////});

	//////if (taskExecutor.start())
	//////{
	//////	ddk::context_future<ddk::iteration_result> __res = taskExecutor <<= highOrderReceiver <<= highOrderIterable;

	//////	__res.wait();

	//////	taskExecutor.stop();
	//////}

	////ddk::make_function([](int i_value) {printf("cur high order alternate value: %d\n",i_value); }) <<= ddk::view::order(ddk::alternate_order(4)) <<= highOrderIterable;
	////ddk::make_function([](int i_value) {printf("cur high transpose dimension value: %d\n",i_value); }) <<= ddk::view::order(ddk::transponse_dimension_order) <<= highOrderProva;

	////std::vector<int> highOrderProvaSuma;
	//////const auto res = ddk::iter::sum <<= ddk::iter::pow(ddk::arg_0,2.f) <<= ddk::iter::sum(ddk::iter::transform([](int i_value) { return 2.f * i_value; }) <<= foo,foo,foo);
	//[](float i_value)
	//{
	//	int a = 0;
	//	++a;
	//} <<= ddk::iter::pow <<= ddk::fusion(foo,foo);
	////highOrderProvaSuma <<= ddk::iter::inv(foo);
	////int provaSuma = ddk::iter::sum(foo);
	////int provaSuma2 = ddk::sum({10,4,-25,1897,76});

	ddk::tuple<double,int,float,std::string> provaTuple;
	provaTuple.set<0>(10.2f);
	provaTuple.set<1>(20);
	provaTuple.set<2>(30.5f);
	provaTuple.set<3>("holaquease");

	////ddk::make_function([](int i_value){}) <<= highOrderProvaSuma;

	////ddk::const_random_access_iterable<ddk::const_variant_const_reference<double,int,float>> provaTuplaIterable = ddk::make_iterable<ddk::const_random_access_iterable<ddk::const_variant_const_reference<double,int,float>>>(provaTuple);

	//////provaSumTuple <<= ddk::iter::sum(provaTuplaIterable,provaTuplaIterable,provaTuplaIterable);

	////[](const ddk::variant_const_reference<double,int,float>& i_value)
	////{
	////	if(i_value.is<const double&>())
	////	{
	////		printf("curr tupla str value: %f\n",i_value.get<const double&>());
	////	}
	////	else if(i_value.is<const int&>())
	////	{
	////		printf("curr tupla int value: %d\n",i_value.get<const int&>());
	////	}
	////	else
	////	{
	////		printf("curr tupla float value: %f\n",i_value.get<const float&>());
	////	}

	////} <<= provaTuple;

	ddk::tuple_visitor tupleVisitor;

	tupleVisitor <<= provaTuple;

	////ddk::const_random_access_iterable<int> fooIterable = ddk::make_iterable<ddk::random_access_iterable<int>>(foo);

	////[](int i_value)
	////{
	////	printf("valor trobat: %d\n",i_value);
	////} <<= ddk::iter::find_if(foo,[](const int& i_value){ return i_value > 1; });

	////auto deducedIterable = ddk::deduce_iterable(foo);
	////auto deducedFooIterable = ddk::deduce_iterable(fooIterable);

	//////ddk::thread myThread;
	//////((ddk::make_function([](ddk::const_bidirectional_value<int> i_value) { printf("1 current value: %d at %zd\n",*i_value,value_position(i_value)); }) <<= fooIterable) -> attach(std::move(myThread))).then([](const ddk::iteration_result&){});

	//////ddk::algo::swap swaper(ddk::make_function([](int i_value){ return i_value == 4; }),ddk::make_function([](int i_value){ return i_value == 76; }));

	////ddk::const_random_access_iterable<int> transformedFoo = ddk::view::filter([](const int& i_value) { return i_value > 0; }) <<= foo;

	//////ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("2 current value: %d at %zd\n", *i_value, value_position(i_value)); }) <<= ddk::view::order(ddk::reverse_order) <<= ddk::view::filter([](const A& i_value) { return i_value > 0; }) <<= ddk::iter::transform([](int i_value) { return A(i_value); }) <<= fooIterable;

}
TEST(DDKIterableTest, iterableUnion)
{
	//char _[4096] = { 0 };

    std::vector<A> foo1;
    foo1.push_back(10);
    foo1.push_back(25);
    foo1.push_back(-4);
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

	[](const A& i_value)
	{
		int a = 0;
		++a;
	}	<<= ddk::view::filter([](const A& i_value) { return i_value > 0; })
		<<= ddk::iter::transform([](const A& i_value) { return i_value + i_value; })
		<<= ddk::view::order(ddk::reverse_order)
		<<= ddk::concat(ddk::deduce_iterable(foo1),ddk::deduce_iterable(foo2),ddk::deduce_iterable(foo3));

	auto kk = ddk::concat(ddk::deduce_iterable(foo1),ddk::deduce_iterable(foo2),ddk::deduce_iterable(foo3));

	kk.iterate_impl(ddk::size_action{});

	[](const A& i_value)
	{
		int a = 0;
		++a;
	}	<<= ddk::iter::transform([](const A& i_val1,const D& i_val2,const E& i_val3) -> A { return i_val1 + i_val2 + i_val3; })
		<<= ddk::view::filter([](const A& i_val1,const D& i_val2,const E& i_val3) { return i_val1 > i_val2; })
		<<= ddk::view::order(ddk::reverse_order)
		<<= ddk::fusion(ddk::deduce_iterable(foo1),ddk::deduce_iterable(foo2),ddk::deduce_iterable(foo3));

	//ddk::tuple<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> fooIterable4(fooIterable1,fooIterable2);
	//ddk::detail::union_iterable_impl<ddk::const_random_access_iterable<A>,ddk::const_random_access_iterable<D>> unionIterable(fooIterable1,fooIterable2);

	//for(const auto& iterable : ddk::view::group_by(ddk::concat(fooIterable1,fooIterable2,fooIterable3),[](const A& i_value){ return *i_value > 100; },[](const A& i_value){ return *i_value < 50; }))
	//{
	//	ddk::make_function([](const A& i_value) { printf("current value: %d\n",*i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= iterable;
	//	printf("done\n");
	//}

	//ddk::iteration_result i_result = [](const A& i_value1,const A& i_value2){ printf("VALORS: %d, %d\n",*i_value1,*i_value2);} <<= ddk::fusion(fooIterable1,fooIterable2);

	////ddk::make_function([](ddk::const_bidirectional_value<const A> i_value){ printf("1 current value: %d at %zd\n",**i_value,value_position(i_value)); }) <<=  ddk::concat(fooIterable1,fooIterable2);
 ////   ddk::const_random_access_iterable<A> fooIterableUnion2 = ddk::concat(fooIterable1,fooIterable2,fooIterable3);
	////ddk::make_function([](ddk::const_bidirectional_value<const A> i_value) { printf("2 current value: %d\n",**i_value); }) <<= ddk::view::order(ddk::reverse_order) <<= fooIterableUnion2;
	//
	//enum class MyError
	//{
	//	A = 100,
	//	B = 200
	//};
	//
	////ddk::iteration_result iterRes = [](ddk::const_bidirectional_value<const A> i_value)
	////{ 
	////	ddk::stop_iteration(MyError::A,"fotli fort");
	////	printf("3 current value: %d\n",**i_value); 
	////} <<= ddk::view::order(ddk::reverse_order) <<= ddk::iter::transform([](const A& i_val1, const D& i_val2, const A& i_val3) { return i_val1; }) <<= ddk::fusion(ddk::concat(fooIterable1,fooIterable2),fooIterable2, ddk::concat(fooIterable1,fooIterable3));

	////if(iterRes != ddk::success)
	////{
	////	const ddk::stop_error& currError = iterRes.error();

	////	const std::string whatStr = currError.what();
	////	const MyError _code = currError.get_code<MyError>();
	////}
}

//std::vector<int> createLargeVector2(size_t size)
//{
//	std::vector<int> res;
//
//	for(int index = 0; index < size; index++)
//	{
//		res.push_back(index);
//	}
//
//	return std::move(res);
//}
//
//std::vector<int> myLargeVector2 = createLargeVector2(1000000);
//
//TEST(DDKIterableTest,directIteration)
//{
//	ddk::make_function([](int i_value) {}) <<= myLargeVector2;
//}
