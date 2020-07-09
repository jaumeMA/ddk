#include <gtest/gtest.h>
#include "shared_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "reference_wrapper.h"

using namespace testing;
using testing::Types;

class EwasSharedPtrtTest : public Test
{
};

TEST(EwasSharedPtrtTest,defaultConstruction)
{
	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo;

		EXPECT_EQ(foo.empty(),true);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,construction1)
{
	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,construction2)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::as_shared_reference(newNestedValue,objFactory);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,construction3)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);
		tagged_pointer<ewas::shared_reference_counter> taggedRefCounter(new ewas::shared_reference_counter(), ewas::ReferenceAllocationType::Dynamic);

		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ewas::as_shared_reference(newNestedValue,taggedRefCounter,objFactory);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,moveConstruction)
{
	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,moveAssignment)
{
	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo2;

		EXPECT_EQ(foo2.empty(),true);

		foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,baseAccess)
{
	{
		ewas::shared_pointer_wrapper<DefaultType> foo = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,assignment)
{
	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> foo;

		foo = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,assignmentBetweenShareds)
{
	ewas::shared_reference_counter refCounter;

	{
		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared;
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.Allocate(0xFF);

		fooShared = ewas::as_shared_reference(newNestedValue,tagged_pointer<ewas::shared_reference_counter>(&refCounter,ewas::ReferenceAllocationType::Embedded));

		EXPECT_EQ(refCounter.getNumSharedReferences(),1);

		{
			ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared1 = fooShared;

			EXPECT_EQ(refCounter.getNumSharedReferences(),2);

			{
				ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared2 = fooShared;

				EXPECT_EQ(refCounter.getNumSharedReferences(),3);

				{
					ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared3 = fooShared;

					EXPECT_EQ(refCounter.getNumSharedReferences(),4);

					{
						ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared4 = fooShared;

						EXPECT_EQ(refCounter.getNumSharedReferences(),5);

						{
							ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> fooShared5 = fooShared;

							EXPECT_EQ(refCounter.getNumSharedReferences(),6);
						}

						EXPECT_EQ(refCounter.getNumSharedReferences(),5);
					}

					EXPECT_EQ(refCounter.getNumSharedReferences(),4);
				}

				EXPECT_EQ(refCounter.getNumSharedReferences(),3);
			}

			EXPECT_EQ(refCounter.getNumSharedReferences(),2);
		}

		EXPECT_EQ(refCounter.getNumSharedReferences(),1);
	}

	EXPECT_EQ(refCounter.getNumSharedReferences(),0);

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,staticCast)
{
	{
		ewas::shared_pointer_wrapper<DefaultType> foo = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> _foo = ewas::static_shared_cast<ConstructionDeletionBalancer>(foo);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(EwasSharedPtrtTest,dynamicCast)
{
	{
		ewas::shared_pointer_wrapper<DefaultType> foo = ewas::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ewas::shared_pointer_wrapper<ConstructionDeletionBalancer> _foo = ewas::dynamic_shared_cast<ConstructionDeletionBalancer>(foo);

		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}