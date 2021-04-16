#include <gtest/gtest.h>
#include "ddk_variant.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_static_visitor.h"
#include "ddk_function.h"

using namespace testing;

template<typename T>
class DDKTypedVariantTest : public Test
{
};

class DDKVariantTest : public Test
{
};

class TestVisitor : public ddk::static_visitor<int>
{
public:
	int operator()(const DefaultType& i_value) const
	{
		return i_value.getValue();
	}
	int operator()(const nonCopyConstructibleType& i_value) const
	{
		return i_value.getValue();
	}
	int operator()(const nonMoveConstructibleType& i_value) const
	{
		return i_value.getValue();
	}
	int operator()(const nonCopyAssignableType& i_value) const
	{
		return i_value.getValue();
	}
	int operator()(const nonMoveAssignableType& i_value) const
	{
		return i_value.getValue();
	}
};

using testing::Types;

TEST(DDKVariantTest,defaultConstruction)
{
	constexpr ddk::variant<int,char> foo(10);
	constexpr ddk::variant<int,char> fooo = constexpr_copy_variant(foo);

	EXPECT_EQ(foo.is<0>(),true);
}
TEST(DDKVariantTest,construction1)
{
	ddk::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);
}
TEST(DDKVariantTest,construction2)
{
	nonCopyConstructibleType nestedFoo(0xFF);
	ddk::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(std::move(nestedFoo));

	EXPECT_EQ(foo.is<nonCopyConstructibleType>(),true);
	EXPECT_EQ(foo.get<nonCopyConstructibleType>(),0xFF);
}
TEST(DDKVariantTest,construction3)
{
	nonMoveConstructibleType nestedFoo(0xFF);
	ddk::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType> foo(nestedFoo);

	EXPECT_EQ(foo.is<nonMoveConstructibleType>(),true);
	EXPECT_EQ(foo.get<nonMoveConstructibleType>(),0xFF);
}
TEST(DDKVariantTest,assignment1)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	foo = DefaultType(0xFF);

	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);
}
TEST(DDKVariantTest,assignment2)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);
	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xFF);
}
TEST(DDKVariantTest,assignment3)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	nonMoveAssignableType nestedFoo(0xFF);
	foo = nestedFoo;

	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
	EXPECT_EQ(foo.get<nonMoveAssignableType>(),0xFF);
}
TEST(DDKVariantTest,reconstruction)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.get<DefaultType>(),0xFF);

	nonCopyAssignableType nestedFoo1(0xEE);
	foo = std::move(nestedFoo1);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);
	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xEE);

	nonMoveAssignableType nestedFoo2(0xDD);
	foo = nestedFoo2;

	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
	EXPECT_EQ(foo.get<nonMoveAssignableType>(),0xDD);
}
TEST(DDKVariantTest,is)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	DefaultType nestedFoo1(0xFF);
	foo = nestedFoo1;

	EXPECT_EQ(foo.is<DefaultType>(),true);
	EXPECT_EQ(foo.is_base_of<DefaultType>(),true);

	nonCopyAssignableType nestedFoo2(0xFF);
	foo = std::move(nestedFoo2);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	nonMoveAssignableType nestedFoo3(0xFF);
	foo = nestedFoo3;

	EXPECT_EQ(foo.is<nonMoveAssignableType>(),true);
}
TEST(DDKVariantTest,get)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	const nonCopyAssignableType& nestedFooRef = foo.get<nonCopyAssignableType>();

	EXPECT_EQ(nestedFooRef,0xFF);
}
TEST(DDKVariantTest,getRef)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xFF);

	nonCopyAssignableType& nestedFooRef = foo.get<nonCopyAssignableType>();

	nestedFooRef.setValue(0xEE);

	EXPECT_EQ(foo.get<nonCopyAssignableType>(),0xEE);
}
TEST(DDKVariantTest,extract)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	nonCopyAssignableType nestedFoo(0xFF);
	foo = std::move(nestedFoo);

	EXPECT_EQ(foo.is<nonCopyAssignableType>(),true);

	nonCopyAssignableType nestedFooRef = std::move(foo).extract<nonCopyAssignableType>();

	EXPECT_EQ(nestedFooRef,0xFF);
}
TEST(DDKVariantTest,comparison)
{
	ddk::variant<DefaultType,nonCopyAssignableType,nonMoveAssignableType> foo(DefaultType(0xFF));

	EXPECT_EQ(foo == DefaultType(0xFF),true);
	EXPECT_EQ(foo == nonCopyAssignableType(0xFF),false);
	EXPECT_EQ(foo == nonMoveAssignableType(0xFF),false);
}
TEST(DDKVariantTest,visitation)
{
	TestVisitor visitor;
	ddk::variant<DefaultType,nonCopyConstructibleType,nonMoveConstructibleType,nonCopyAssignableType,nonMoveAssignableType> foo;

	EXPECT_EQ(foo.is<0>(),true);

	DefaultType nestedValue1(0xFF);
	foo = nestedValue1;

	EXPECT_EQ(foo.visit<TestVisitor>(),0xFF);

	nonCopyConstructibleType nestedValue2(0xEE);
	foo = std::move(nestedValue2);

	EXPECT_EQ(foo.visit(visitor),0xEE);

	nonMoveConstructibleType nestedValue3(0xDD);
	foo = nestedValue3;

	EXPECT_EQ(foo.visit(visitor),0xDD);

	nonCopyAssignableType nestedValue4(0xCC);
	foo = std::move(nestedValue4);

	EXPECT_EQ(foo.visit(visitor),0xCC);

	nonMoveAssignableType nestedValue5(0xBB);
	foo = nestedValue5;

	EXPECT_EQ(foo.visit(visitor),0xBB);
}

struct myMultiVisitor
{
	typedef size_t return_type;

	template<typename ... T>
	size_t operator()(T&& ... i_values) const
	{
		return ddk::mpl::get_num_types<T...>();
	}
};

TEST(DDKVariantTest,multi_visitation)
{
	ddk::variant<std::string,int,size_t,nonCopyConstructibleType> foo1 = "holaquease";
	ddk::variant<DefaultType,std::vector<int>> foo2 = { 10 };
	ddk::variant<char,double,float,std::string> foo3 = 0.15f;

	const bool res1 = ddk::visit([](auto&& ... i_value) -> bool { return true; },foo1,foo2,foo3);
	const size_t res2 = ddk::visit<myMultiVisitor>(foo1,foo2,foo3);
}
