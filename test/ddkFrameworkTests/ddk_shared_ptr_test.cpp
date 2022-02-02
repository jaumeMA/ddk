#include <gtest/gtest.h>
#include "ddk_shared_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fixed_size_allocator.h"

using namespace testing;
using testing::Types;

class DDKSharedPtrtTest : public Test
{
};

TEST(DDKSharedPtrtTest,defaultConstruction)
{
	{
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo1;

		EXPECT_EQ(foo1.empty(),true);

		ddk::shared_reference_wrapper<ConstructionDeletionBalancer> _foo1 = ddk::promote_to_ref(foo1);

		ddk::atomic_shared_pointer_wrapper<ConstructionDeletionBalancer> foo2;
		ddk::atomic_shared_reference_wrapper<ConstructionDeletionBalancer> _foo2 = ddk::promote_to_ref(foo2);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,construction1)
{
	{
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,construction2)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.allocate(0xFF);
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::as_shared_reference(newNestedValue,objFactory);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,construction3)
{
	{
		typedef ddk::shared_control_block<ConstructionDeletionBalancer,TestDynamicDeleter<ConstructionDeletionBalancer>> control_block;
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ConstructionDeletionBalancer* newNestedValue = objFactory.allocate(0xFF);
		ddk::tagged_pointer<ddk::shared_reference_counter> taggedRefCounter(new control_block(newNestedValue,objFactory), ddk::ReferenceAllocationType::Dynamic);

		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo = ddk::as_shared_reference(newNestedValue,taggedRefCounter);


		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,moveConstruction)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory;
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ddk::make_shared_reference<ConstructionDeletionBalancer>(objFactory,0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,moveAssignment)
{
	{
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo1 = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo1.empty(),false);
		EXPECT_EQ(foo1->getValue(),0xFF);

		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo2;

		EXPECT_EQ(foo2.empty(),true);

		foo2 = std::move(foo1);

		EXPECT_EQ(foo1.empty(),true);
		EXPECT_EQ(foo2.empty(),false);
		EXPECT_EQ(foo2->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,baseAccess)
{
	{
		ddk::shared_pointer_wrapper<DefaultType> foo = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,assignment)
{
	{
		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> foo;

		foo = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(*foo,0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,assignmentBetweenShareds)
{
	{
		TestDynamicFactory<ConstructionDeletionBalancer> objFactory; 
		ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared;
		ConstructionDeletionBalancer* newNestedValue = objFactory.allocate(0xFF);
		ddk::distributed_control_block<ConstructionDeletionBalancer,TestDynamicDeleter<ConstructionDeletionBalancer>> refCounter(newNestedValue,objFactory);

		fooShared = ddk::as_shared_reference(newNestedValue, ddk::tagged_pointer<ddk::distributed_reference_counter>(&refCounter,ddk::ReferenceAllocationType::Embedded));

		EXPECT_EQ(refCounter.getNumSharedReferences(),1);

		{
			ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared1 = fooShared;

			EXPECT_EQ(refCounter.getNumSharedReferences(),2);

			{
				ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared2 = fooShared;

				EXPECT_EQ(refCounter.getNumSharedReferences(),3);

				{
					ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared3 = fooShared;

					EXPECT_EQ(refCounter.getNumSharedReferences(),4);

					{
						ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared4 = fooShared;

						EXPECT_EQ(refCounter.getNumSharedReferences(),5);

						{
							ddk::distributed_pointer_wrapper<ConstructionDeletionBalancer> fooShared5 = fooShared;

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

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,staticCast)
{
	{
		ddk::shared_pointer_wrapper<DefaultType> foo = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> _foo = ddk::static_shared_cast<ConstructionDeletionBalancer>(foo);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}
TEST(DDKSharedPtrtTest,dynamicCast)
{
	{
		ddk::shared_pointer_wrapper<DefaultType> foo = ddk::make_shared_reference<ConstructionDeletionBalancer>(0xFF);

		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(foo->getValue(),0xFF);

		ddk::shared_pointer_wrapper<ConstructionDeletionBalancer> _foo = ddk::dynamic_shared_cast<ConstructionDeletionBalancer>(foo);

		EXPECT_EQ(_foo.empty(),false);
		EXPECT_EQ(foo.empty(),false);
		EXPECT_EQ(_foo->getValue(),0xFF);
	}

	EXPECT_EQ(ConstructionDeletionBalancer::isBalanced(),true);
}