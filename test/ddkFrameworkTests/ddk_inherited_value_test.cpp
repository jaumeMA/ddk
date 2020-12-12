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

class DerivedBaseType;

struct BaseTypeVisitor : public ddk::dynamic_visitor
{
	DECLARE_TYPE_VISITOR_BASE(BaseTypeVisitor,BaseType)

public:
	virtual ddk::any_value visit(const BaseType&) const
	{
		return true;
	}
	virtual ddk::any_value visit(BaseType&) const
	{
		return true;
	}
	virtual ddk::any_value visit(const DerivedBaseType&) const
	{
		return true;
	}
	virtual ddk::any_value visit(DerivedBaseType&) const
	{
		return true;
	}
};

struct DerivedTypeVisitor : public BaseTypeVisitor
{
	ddk::any_value visit(const BaseType&) const override
	{
		return true;
	}
	ddk::any_value visit(BaseType&) const override
	{
		return true;
	}
	ddk::any_value visit(const DerivedBaseType&) const override
	{
		return true;
	}
	ddk::any_value visit(DerivedBaseType&) const override
	{
		return true;
	}
};

template<typename T>
struct DerivedBaseTypeTraits
{
	typedef BaseType type_interface;
	typedef BaseTypeVisitor visitor_interface;
	typedef T final_type;
};

class DerivedBaseType : public BaseType
{
	DECLARE_TYPE_VISITABLE(DerivedBaseType,DerivedBaseTypeTraits<DerivedBaseType>)

public:
	using BaseType::BaseType;
};

class DDKInheritedValuetTest : public Test
{
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::inherited_value<BaseType> foo = ddk::make_inherited_value<DerivedBaseType>(10);

	DerivedTypeVisitor visitor;
	if(foo.may_visit<DerivedTypeVisitor>())
	{
		ddk::any_value visitRes = foo.visit(visitor);
	}
}
