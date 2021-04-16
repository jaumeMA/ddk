#include <gtest/gtest.h>
#include "ddk_inherited_value.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_rtti.h"
#include "ddk_any_value.h"
#include "ddk_dynamic_callable.h"

using namespace testing;
using testing::Types;

namespace prova
{


class BaseType1
{
public:
	BaseType1(int i_value = 0)
	: m_value(i_value)
	{
	}
	virtual ~BaseType1() = default;

private:
	int m_value;
};

class BaseType2
{
public:
	BaseType2(int i_value = 0)
	: m_value(i_value)
	{
	}
	virtual ~BaseType2() = default;

private:
	int m_value;
};

class DerivedBaseType1 : public BaseType1, public BaseType2
{
public:
	DerivedBaseType1(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType1,BaseType1,BaseType2);

class DerivedBaseType2: public BaseType1, public BaseType2
{
public:
	DerivedBaseType2(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType2,BaseType1,BaseType2);

}

class DDKInheritedValuetTest : public Test
{
};

struct DerivedTypeMultiVisitor
{
	typedef int return_type;

	return_type operator()(const prova::DerivedBaseType1&,const prova::DerivedBaseType1&) const
	{
		return 1;
	}
	return_type operator()(const prova::DerivedBaseType1&,const prova::DerivedBaseType2&) const
	{
		return 1;
	}
	template<typename T1, typename T2>
	return_type operator()(const T1& i_lhs, const T2& i_rhs)
	{
	    return 0;
	}
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::inherited_value<prova::BaseType1> foo0 = ddk::make_inherited_value<prova::DerivedBaseType1>(10);
	ddk::inherited_value<prova::BaseType1> foo1 = ddk::make_inherited_value<prova::DerivedBaseType2>(20);
	DerivedTypeMultiVisitor multiVisitor;

	ddk::dynamic_callable<bool,prova::BaseType1>([](auto&& i_value){ return true; });

	int a = 0;

	ddk::visit<void,prova::BaseType1>([](auto&& i_lhs, auto&& i_rhs){ return 0; },foo0,foo1);
	int b = 0;
}
