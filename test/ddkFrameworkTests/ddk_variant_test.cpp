#include <gtest/gtest.h>
#include "ewas_variant.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ewas_static_visitor.h"

using namespace testing;

template<typename T>
class EwasTypedVariantTest : public Test
{
};

class EwasVariantTest : public Test
{
};

class TestVisitor : public ewas::static_visitor<int>
{
public:
	int visit(const DefaultType& i_value)
	{
		return i_value.getValue();
	}
	int visit(const nonCopyConstructibleType& i_value)
	{
		return i_value.getValue();
	}
	int visit(const nonMoveConstructibleType& i_value)
	{
		return i_value.getValue();
	}
	int visit(const nonCopyAssignableType& i_value)
	{
		return i_value.getValue();
	}
	int visit(const nonMoveAssignableType& i_value)
	{
		return i_value.getValue();
	}
};


using testing::Types;

TEST(EwasVariantTest,defaultConstruction)
{
	ewas::variant<int,char,std::string> foo;

	EXPECT_EQ(foo.empty(),true);
}
TEST(EwasVariantTest,construction1)
{
	ewas::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);
}
TEST(EwasVariantTest,construction2)
{
	nonCopyConstructibleType nestedFoo(0xFF);
	ewas::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(std::move(nestedFoo));

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyConstructibleType>(),true);
	EXPECT_EQ(foo.get<nonCopyConstructibleType>(),0xFF);
}
TEST(EwasVariantTest,construction3)
{
	nonMoveConstructibleType nestedFoo(0xFF);
	ewas::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(nestedFoo);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonMoveConstructibleType>(),true);
	EXPECT_EQ(foo.get<nonMoveConstructibleType>(),0xFF);
}
TEST(EwasVariantTest,assignment1)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	foo = DefaultType(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);
}
TEST(EwasVariantTest,assignment2)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);
	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xFF);
}
TEST(EwasVariantTest,assignment3)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonMoveAssignableType nestedFoo(0xFF);
	foo = nestedFoo;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
	EXPECT_EQ(foo.get<nonMoveAssignableType>(),0xFF);
}
TEST(EwasVariantTest,reconstruction)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);

	nonCopyAssignableType nestedFoo1(0xEE);
	foo = std::move(nestedFoo1);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);
	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xEE);

	nonMoveAssignableType nestedFoo2(0xDD);
	foo = nestedFoo2;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
	EXPECT_EQ(foo.get<nonMoveAssignableType>(),0xDD);
}
TEST(EwasVariantTest,is)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	DefaultType nestedFoo1(0xFF);
	foo = nestedFoo1;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<DefaultType>(),true);

	nonCopyAssignableType nestedFoo2(0xFF);
	foo = std::move(nestedFoo2);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	nonMoveAssignableType nestedFoo3(0xFF);
	foo = nestedFoo3;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
}
TEST(EwasVariantTest,get)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	const nonCopyAssignableType& nestedFooRef = foo.get<nonCopyAssignableType>();

	EXPECT_EQ(nestedFooRef,0xFF);
}
TEST(EwasVariantTest,getRef)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xFF);

	nonCopyAssignableType& nestedFooRef = foo.get<nonCopyAssignableType>();

	nestedFooRef.setValue(0xEE);

	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xEE);
}
TEST(EwasVariantTest,extract)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	nonCopyAssignableType nestedFooRef = foo.extract<nonCopyAssignableType>();

	EXPECT_EQ(foo.empty(),true);
	EXPECT_EQ(nestedFooRef,0xFF);
}
TEST(EwasVariantTest,comparison)
{
	ewas::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo == DefaultType(0xFF),true);
	EXPECT_EQ(foo == nonCopyAssignableType(0xFF),false);
	EXPECT_EQ(foo == nonMoveAssignableType(0xFF),false);
}
TEST(EwasVariantTest,visitation)
{
	TestVisitor visitor;
	ewas::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	DefaultType nestedValue1(0xFF);
	foo = nestedValue1;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.visit(visitor),0xFF);

	nonCopyConstructibleType nestedValue2(0xEE);
	foo = std::move(nestedValue2);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.visit(visitor),0xEE);

	nonMoveConstructibleType nestedValue3(0xDD);
	foo = nestedValue3;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.visit(visitor),0xDD);

	nonCopyAssignableType nestedValue4(0xCC);
	foo = std::move(nestedValue4);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.visit(visitor),0xCC);

	nonMoveAssignableType nestedValue5(0xBB);
	foo = nestedValue5;

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.visit(visitor),0xBB);
}