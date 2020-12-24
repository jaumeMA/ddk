#include <gtest/gtest.h>
#include "ddk_co_iterable.h"
#include "test_utils.h"
#include <vector>

using namespace testing;

class DDKCoIterableTest : public Test
{
};

template<typename T>
class MyIterable
{
	DDK_ITERABLE_TYPE(MyIterable,MyIterableAdaptor,std::forward_iterator_tag)
public:
	typedef const size_t& reference;
	typedef const size_t& const_reference;

	MyIterable(size_t i_init, size_t i_end)
	: m_init(i_init)
	, m_end(i_end)
	{
		DDK_ASSERT(m_init < m_end, "you shall provide ordered extremes");
	}
	size_t get_min() const
	{
		return m_init;
	}
	size_t get_max() const
	{
		return m_end;
	}

private:
	size_t m_init;
	size_t m_end;
};

class MyIterableAdaptor
{
public:
	typedef typename MyIterable<int>::reference reference;
	typedef typename MyIterable<int>::const_reference const_reference;

	MyIterableAdaptor(const MyIterable<int>& i_iterable,const ddk::iter::shift_action& i_initialAction);
	inline const_reference get_value() const noexcept;
	inline ddk::optional<const_reference> next_value() const noexcept;
	inline bool valid() const noexcept;

private:
	const MyIterable<int>& m_iterable;
	mutable size_t m_currValue = 0;
};

MyIterableAdaptor::MyIterableAdaptor(const MyIterable<int>& i_iterable,const ddk::iter::shift_action& i_initialAction)
: m_iterable(i_iterable)
,m_currValue(i_initialAction.shifted())
{
}
bool MyIterableAdaptor::valid() const noexcept
{
	return m_currValue != ddk::iter::iterable_state::npos;
}
typename MyIterableAdaptor::const_reference MyIterableAdaptor::get_value() const noexcept
{
	return m_currValue;
}
ddk::optional<typename MyIterableAdaptor::const_reference> MyIterableAdaptor::next_value() const noexcept
{
	if(m_currValue < m_iterable.get_max())
	{
		return ++m_currValue;
	}
	else
	{
		return none;
	}
}


class Prova
{
public:
	Prova(const std::string& i_str)
	: m_data(i_str)
	{
	}
	Prova(const Prova&) = default;
	Prova(Prova&& other)
	: m_data(std::move(other.m_data))
	{
	}

	void foo() &&
	{
		int a = 0;
	}

private:
	std::string m_data;
};

TEST(DDKCoIterableTest,stdVectorForwardIteration)
{
	typedef std::vector<int> container;
	typedef ddk::co_iterable<container> iterable;
	typedef typename iterable::iterator iterator;

	container kkk;
	container kk1;

	Prova prova("lolailo");

	std::move(prova).foo();

	kkk.push_back(10);
	kk1.push_back(10);
	kk1.push_back(2);
	kk1.push_back(567);
	kk1.push_back(22);
	kk1.push_back(7);
	kk1.push_back(-160);
	kk1.push_back(2345);

	printf("first\n");
	{
		typedef ddk::co_iterable<container> const_iterable;
		const_iterable res = ddk::co_iterate(kkk);
		typedef typename const_iterable::iterator const_iterator;
		const_iterator itRes = std::begin(res);
		for(size_t index = 0; itRes != std::end(res); ++itRes,++index)
		{
			const int& res = *itRes;
			printf("checking %d\n",res);
			EXPECT_EQ(*itRes,kk1[index]);
		}
	}

	printf("second\n");
	{
		iterable res1 = ddk::co_iterate(kk1);
		iterator itRes1 = std::begin(res1);

		for(size_t index = 0; itRes1 != std::end(res1); ++itRes1,++index)
		{
			int& res = *itRes1;
			EXPECT_EQ(*itRes1,kk1[index]);
		}

		iterable res3 = ddk::co_iterate(kk1);
		iterator itRes3 = std::begin(res3);
		for(size_t index = 0; index < kk1.size(); ++index)
		{
			EXPECT_EQ(*(itRes3[index]),kk1[index]);
		}

		iterable res2 = ddk::co_iterate(kk1);
		iterator itRes2 = std::begin(res2);

		for(size_t index=0;itRes2!=std::end(res2);++itRes2,++index)
		{
			int& res = *itRes2;

			EXPECT_EQ(*itRes2, kk1[index]);
		}
	}

	{
		iterable res4 = ddk::co_iterate(kk1);
		iterator itRes4 = std::begin(res4);
		for (int index = 0; index < kk1.size(); ++index)
		{
			EXPECT_EQ(*(itRes4 + index), kk1[index]);
		}
	}

	{
		typedef std::vector<int> container;
		container kkkk;

		{
			typedef ddk::co_iterable<container> const_iterable;
			const_iterable res = ddk::co_iterate(kkkk);
			typedef typename const_iterable::iterator const_iterator;
			const_iterator itRes = std::begin(res);
			for(size_t index = 0; itRes != std::end(res); ++itRes,++index)
			{
				const int& res = *itRes;
				EXPECT_EQ(*itRes,kk1[index]);
			}
		}
	}
}
TEST(DDKCoIterableTest, consStdVectorForwardIteration)
{
	typedef std::vector<int> container;
	const container kk1(10);

	{
		typedef ddk::co_iterable<const container> const_iterable;
		const_iterable res = ddk::co_iterate(kk1);
		typedef typename const_iterable::const_iterator const_iterator;
		const_iterator itRes = std::begin(res);
		for (size_t index = 0; itRes != std::end(res); ++itRes, ++index)
		{
			const int& res = *itRes;
			EXPECT_EQ(*itRes, kk1[index]);
		}
	}
}
TEST(DDKCoIterableTest, stdVectorConstForwardIteration)
{
	typedef std::vector<int> container;
	container kk1;

	kk1.push_back(10);
	kk1.push_back(2);
	kk1.push_back(567);
	kk1.push_back(22);
	kk1.push_back(7);
	kk1.push_back(-160);
	kk1.push_back(2345);

	typedef ddk::co_iterable<container> iterable;
	const iterable res = ddk::co_iterate(kk1);
	typedef typename iterable::const_iterator const_iterator;
	const_iterator itRes = std::begin(res);
	for (size_t index = 0; itRes != std::end(res); ++itRes, ++index)
	{
		const int& res = *itRes;
		EXPECT_EQ(*itRes, kk1[index]);
	}
}
TEST(DDKCoIterableTest, assignIterator)
{
	typedef std::vector<int> container;
	typedef ddk::co_iterable<container> iterable;
	typedef typename iterable::iterator iterator;

	container kk0;

	kk0.push_back(10);
	kk0.push_back(2);
	kk0.push_back(567);
	kk0.push_back(22);
	kk0.push_back(7);
	kk0.push_back(-160);
	kk0.push_back(2345);

	iterable res0 = ddk::co_iterate(kk0);
	iterator itZero = std::begin(res0);

	container kk1;

	kk1.push_back(10);
	kk1.push_back(2);
	kk1.push_back(567);
	kk1.push_back(22);
	kk1.push_back(7);
	kk1.push_back(-160);
	kk1.push_back(2345);

	iterable res1 = ddk::co_iterate(kk1);
	iterator itFirst = std::begin(res1);

	container kk2;

	kk2.push_back(10);
	kk2.push_back(2);
	kk2.push_back(567);
	kk2.push_back(22);
	kk2.push_back(7);
	kk2.push_back(-160);
	kk2.push_back(2345);

	iterable res2 = ddk::co_iterate(kk2);
	iterator itSecond = std::begin(res2);

	++itSecond;
	++itSecond;

	printf("done\n");

	iterator itThird = itSecond;

	EXPECT_EQ(*itThird, 567);

	++itThird;
	++itThird;

	EXPECT_EQ(*itThird, 7);
}

std::vector<int> createLargeVector(size_t size)
{
	std::vector<int> res;
		
	for(size_t index = 0; index < size; index++)
	{
		res.push_back(index);
	}

	return std::move(res);
}

std::vector<int> myLargeVector = createLargeVector(1000000);

TEST(DDKCoIterableTest,stdForwardIteration)
{
	//int sum = 0;
	std::vector<int>::const_iterator itQQ = myLargeVector.begin();
	for(size_t index = 0; itQQ != myLargeVector.end(); ++itQQ,++index)
	{
		//printf("curr value: %d\n",*itQQ);
	}
}

TEST(DDKCoIterableTest,myIterableForwardIteration)
{
	typedef ddk::co_iterable<std::vector<int>> iterable;
	typedef typename iterable::iterator iterator;
	const size_t initIndex = 0;
	const size_t maxIndex = 1000000;
	MyIterable<int> foo(initIndex,maxIndex);

	auto res1 = ddk::co_iterate(foo);
	auto itRes1 = std::begin(res1);
	auto itEnd = std::end(res1);
	int a = 0;
	for(; itRes1 != itEnd; ++itRes1)
	{
		//printf("curr value %d\n",*itRes1);
	}

	int b = a;
}