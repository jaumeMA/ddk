#include <gtest/gtest.h>
#include "ddk_unique_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_reference_wrapper.h"

using namespace testing;
using testing::Types;

class DDKUniquePtrtTest : public Test
{
};

TEST(DDKUniquePtrTest,defaultConstruction)
{
	{
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo;

		EXPECT_EQ(foo.empty(),true);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,construction1)
{
	{
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,construction2)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::as_unique_reference(newNestedValue,{ddk::lend(objFactory),ddk::AllocationMode::ConstructionProvided});

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,construction3)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		ddk::tagged_pointer<ddk::unique_reference_counter> taggedRefCounter(new ddk::unique_reference_counter(), ddk::ReferenceAllocationType::Dynamic);

		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::as_unique_reference(newNestedValue,taggedRefCounter,{ ddk::lend(objFactory),ddk::AllocationMode::ConstructionProvided });

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,moveConstruction)
{
	{
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,moveAssignment)
{
	{
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo2;

		EXPECT_EQ(foo2.empty(),true);

		foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,baseAccess)
{
	{
		ddk::unique_pointer_wrapper<DefaultType> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,get)
{
	{
		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,deletion)
{
	ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(*foo,0xFF);

	foo.clear();

	EXPECT_EQ(foo.empty(),true);
	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,staticCast)
{
	{
		ddk::unique_pointer_wrapper<DefaultType> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> _foo = ddk::static_unique_cast<ConstructionDeletionBalancer>(std::move(foo));

		EXPECT_EQ(foo.empty(),true);
		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKUniquePtrTest,dynamicCast)
{
	{
		ddk::unique_pointer_wrapper<DefaultType> foo = ddk::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ddk::unique_pointer_wrapper<ConstructionDeletionBalancer> _foo = ddk::dynamic_unique_cast<ConstructionDeletionBalancer>(std::move(foo));

		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(foo.empty(),true);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}