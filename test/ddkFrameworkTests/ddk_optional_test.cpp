#include <gtest/gtest.h>
#include "ddk_optional.h"
#include <utility>
#include <string>
#include "test_utils.h"

using namespace testing;

template<typename T>
class DDKTypedOptionalTest : public Test
{
};

class DDKOptionalTest : public Test
{
};

using testing::Types;

typedef Types<int, std::string, nonMoveConstructibleType, nonCopyConstructibleType,nonMoveAssignableType,nonCopyAssignableType> Implementations;

TYPED_TEST_SUITE(DDKTypedOptionalTest, Implementations);

TYPED_TEST(DDKTypedOptionalTest,defaultConstruction)
{
	ddk::optional<TypeParam> foo;

	EXPECT_EQ(foo.empty(),true);
}
TEST(DDKOptionalTest,construction1)
{
	ddk::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,construction2)
{
	ddk::optional<nonCopyConstructibleType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,construction3)
{
	ddk::optional<nonMoveConstructibleType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,assignment1)
{
	ddk::optional<DefaultType> foo;

	EXPECT_EQ(foo.empty(),true);

	foo = 0xFF;

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,assignment2)
{
	ddk::optional<nonCopyAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,assignment3)
{
	ddk::optional<nonMoveAssignableType> foo;

	EXPECT_EQ(foo.empty(),true);

	nonMoveAssignableType nestedFoo(0xFF);
	foo = nestedFoo;

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,get1)
{
	ddk::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);
}
TEST(DDKOptionalTest,get2)
{
	ddk::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(*foo,0xFF);
}
TEST(DDKOptionalTest,accessor)
{
	ddk::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo->getValue(),0xFF);
}
TEST(DDKOptionalTest,extraction)
{
	ddk::optional<DefaultType> foo(0xFF);

	EXPECT_EQ(foo.empty(),false);

	EXPECT_EQ(foo.get(),0xFF);

	const DefaultType value = std::move(foo).extract();

	EXPECT_EQ(foo.empty(),true);

	EXPECT_EQ(value,0xFF);
}
TEST(DDKOptionalTest, constExprCopyConstruction)
{
	constexpr ddk::optional<int> foo(0xFF);
	constexpr ddk::optional<long long int> _foo(foo);

	static_assert(*foo == 0xFF, "wtf");
	static_assert(*_foo == 0xFF, "wtf");
}
TYPED_TEST(DDKTypedOptionalTest,OptionalCompilationError)
{
	ddk::optional<TypeParam> foo1;
	//ddk::optional<TypeParam> foo2 = std::move(foo1); //this line causes compilation error for those non construct/assign movable

	//foo1 = std::move(foo2); //this line causes compilation error for those non construct/assign movable

	EXPECT_EQ(foo1.empty(),true);
	//EXPECT_EQ(foo2.empty(),true);
}