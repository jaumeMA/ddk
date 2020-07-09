#include <gtest/gtest.h>
#include "unique_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "reference_wrapper.h"

using namespace testing;
using testing::Types;

class EwasUniquePtrtTest : public Test
{
};

TEST(EwasUniquePtrTest,defaultConstruction)
{
	{
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo;

		EXPECT_EQ(foo.empty(),true);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,construction1)
{
	{
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,construction2)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::as_unique_reference(newNestedValue,objFactory);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,construction3)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		tagged_pointer<ewas::reference_counter<ewas::Policy::Unique>> taggedRefCounter(new ewas::reference_counter<ewas::Policy::Unique>(), ewas::ReferenceAllocationType::Dynamic);

		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::as_unique_reference(newNestedValue,taggedRefCounter,objFactory);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,moveConstruction)
{
	{
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,moveAssignment)
{
	{
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo2;

		EXPECT_EQ(foo2.empty(),true);

		foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,baseAccess)
{
	{
		ewas::unique_pointer_wrapper<DefaultType> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,get)
{
	{
		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,deletion)
{
	ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

	EXPECT_EQ(foo.empty(),false);
	EXPECT_EQ(*foo,0xFF);

	foo.clear();

	EXPECT_EQ(foo.empty(),true);
	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,staticCast)
{
	{
		ewas::unique_pointer_wrapper<DefaultType> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> _foo = ewas::static_unique_cast<ConstructionDeletionBalancer>(std::move(foo));

		EXPECT_EQ(foo.empty(),true);
		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasUniquePtrTest,dynamicCast)
{
	{
		ewas::unique_pointer_wrapper<DefaultType> foo = ewas::make_unique_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ewas::unique_pointer_wrapper<ConstructionDeletionBalancer> _foo = ewas::dynamic_unique_cast<ConstructionDeletionBalancer>(std::move(foo));

		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(foo.empty(),true);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}