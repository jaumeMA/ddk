#include <gtest/gtest.h>
#include "ewas_optional.h"
#include <utility>
#include <string>
#include "test_utils.h"

using namespace testing;

template<typename T>
class EwasTypedOptionalTest : public Test
{
};

class EwasOptionalTest : public Test
{
};

using testing::Types;

typedef Types<int, std::string, nonMoveConstructibleType, nonCopyConstructibleType,nonMoveAssignableType,nonCopyAssignableType> Implementations;

TYPED_TEST_SUITE(EwasTypedOptionalTest, Implementations);

TYPED_TEST(EwasTypedOptionalTest,defaultConstruction)
{
	ewas::optional<TypeParam> foo;

	EXPECT_EQ(foo.empty(),true);
}
TEST(EwasOptionalTest,construction1)
{
	ewas::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,construction2)
{
	ewas::optional<nonCopyConstructibleType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,construction3)
{
	ewas::optional<nonMoveConstructibleType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,assignment1)
{
	ewas::optional<DefaultType> foo;

	EXPECT_EQ(foo.empty(),true);

	foo = 0xFF;

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,assignment2)
{
	ewas::optional<nonCopyAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,assignment3)
{
	ewas::optional<nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonMoveAssignableType nestedFoo(0xFF);
	foo = nestedFoo;

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,get1)
{
	ewas::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(EwasOptionalTest,get2)
{
	ewas::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(*foo,0xFF);
}
TEST(EwasOptionalTest,accessor)
{
	ewas::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo->getValue(),0xFF);
}
TEST(EwasOptionalTest,extraction)
{
	ewas::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);

	const DefaultType value = foo.extract();

	EXPECT_EQ(foo.empty(),true);

	EXPECT_EQ(value,0xFF);
}
TYPED_TEST(EwasTypedOptionalTest,OptionalCompilationError)
{
	ewas::optional<TypeParam> foo1;
	//ewas::optional<TypeParam> foo2 = std::move(foo1); //this line causes compilation error for those non construct/assign movable

	//foo1 = std::move(foo2); //this line causes compilation error for those non construct/assign movable

	EXPECT_EQ(foo1.empty(),true);
	//EXPECT_EQ(foo2.empty(),true);
}