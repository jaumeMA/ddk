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
	DECLARE_TYPE_VISITABLE(DerivedBaseType1)

public:
	using BaseType::BaseType;
};

class DerivedBaseType2: public BaseType
{
	DECLARE_TYPE_VISITABLE(DerivedBaseType2)

public:
	using BaseType::BaseType;
};

class DDKInheritedValuetTest : public Test
{
};

struct DerivedTypeMultiVisitor
{
	typedef ddk::tuple<DerivedBaseType1,DerivedBaseType2> considered_types;
	typedef int return_type;

	int visit(const DerivedBaseType1&,const DerivedBaseType1&) const
	{
		return 1;
	}
	int visit(const DerivedBaseType1&,const DerivedBaseType2&) const
	{
		return 1;
	}
	template<typename ... T>
	int visit(const T& ... i_values) const
	{
		return 0;
	}
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::inherited_value<DerivedBaseType1> foo0 = ddk::make_inherited_value<DerivedBaseType1>(10);
	ddk::inherited_value<DerivedBaseType2> foo1 = ddk::make_inherited_value<DerivedBaseType2>(10);
	DerivedTypeMultiVisitor multiVisitor;

	int res = ddk::visit(multiVisitor,foo0,foo1);
	int a = 0;
}
