#include <gtest/gtest.h>
#include "ddk_co_iterable.h"
#include "test_utils.h"
#include <vector>

using namespace testing;

class DDKCoIterableTest : public Test
{
};

struct MyIterable
{
	typedef size_t& reference;
	typedef const size_t& const_reference;

	//you shall provide an overload of forward_iterator_awaitable for your custom container
	friend inline size_t& forward_iterator_awaitable(MyIterable& i_iterable)
	{
		size_t value = i_iterable.m_init;

		while(value <= i_iterable.m_end)
		{
			ddk::yield(value);

			++value;
		}

		ddk::suspend();

		return ddk::crash_on_return<size_t&>::value();
	}
	friend inline size_t& backward_iterator_awaitable(MyIterable& i_iterable)
	{
		size_t value = i_iterable.m_end;

		while(value >= i_iterable.m_init)
		{
			ddk::yield(value);

			--value;
		}

		ddk::suspend();

		return ddk::crash_on_return<size_t&>::value();
	}

	MyIterable(size_t i_init, size_t i_end)
	: m_init(i_init)
	, m_end(i_end)
	{
		DDK_ASSERT(m_init < m_end, "you shall provide ordered extremes");
	}

private:
	size_t m_init;
	size_t m_end;
};


TEST(DDKCoIterableTest,stdVectorForwardIteration)
{
	std::vector<int> kk1;

	kk1.push_back(10);
	kk1.push_back(2);
	kk1.push_back(567);
	kk1.push_back(22);
	kk1.push_back(7);
	kk1.push_back(-160);
	kk1.push_back(2345);

	auto res = ddk::co_iterate(kk1);
	auto itRes = begin(res);
	for(size_t index=0;itRes != end(res);++itRes,++index)
	{
		EXPECT_EQ(*itRes, kk1[index]);
	}
}
TEST(DDKCoIterableTest,myIterableForwardIteration)
{
	const size_t initIndex = 0;
	const size_t maxIndex = 100;
	MyIterable foo(initIndex,maxIndex);

	size_t currIndex = initIndex;

	for(auto&& itKK : ddk::co_iterate(foo))
	{
		EXPECT_EQ(itKK,currIndex);

		++currIndex;
	}
}
