#include <gtest/gtest.h>
#include "ddk_inherited_value.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_visitable_type.h"
#include "ddk_any_value.h"

using namespace testing;
using testing::Types;

class BaseType
{
	DECLARE_TYPE_VISITABLE_BASE(BaseType)

public:
	BaseType(int i_value = 0)
	: m_value(i_value)
	{
	}
	virtual ~BaseType() = default;
	int getValue() const
	{
		return m_value;
	}

private:
	int m_value;
};

class DerivedBaseType1 : public BaseType
{
	DECLARE_TYPE_VISITABLE(DerivedBaseType1,BaseType)

public:
	using BaseType::BaseType;
};

class DerivedBaseType2: public BaseType
{
	DECLARE_TYPE_VISITABLE(DerivedBaseType2,BaseType)

public:
	using BaseType::BaseType;
};

class DDKInheritedValuetTest : public Test
{
};

struct DerivedTypeMultiVisitor : public ddk::dynamic_visitor<BaseType>
{
	typedef int return_type;

	return_type operator()(const DerivedBaseType1&,const DerivedBaseType1&) const
	{
		return 1;
	}
	return_type operator()(const DerivedBaseType1&,const DerivedBaseType2&) const
	{
		return 1;
	}
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::inherited_value<DerivedBaseType1> foo0 = ddk::make_inherited_value<DerivedBaseType1>(10);
	ddk::inherited_value<DerivedBaseType2> foo1 = ddk::make_inherited_value<DerivedBaseType2>(10);
	DerivedTypeMultiVisitor multiVisitor;

	int a = 0;
	int res = ddk::visit(multiVisitor,foo0,foo1);
	int b = 0;
}
