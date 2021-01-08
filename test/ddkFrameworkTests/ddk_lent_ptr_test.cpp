#include <gtest/gtest.h>
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_unique_pointer_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_reference_wrapper.h"

using namespace testing;
using testing::Types;

class DDKUniquePtrtTest : public Test
{
};

TEST(DDKLentPtrTest,defaultConstruction)
{
	ddk::lent_pointer_wrapper<DefaultType> foo(nullptr);

	EXPECT_EQ(foo==nullptr,true);
}
TEST(DDKLentPtrTest,uniquePtrLentConstruction)
{
	ddk::unique_pointer_wrapper<DefaultType> fooUnique = ddk::make_unique_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo = ddk::lend(fooUnique);

	EXPECT_EQ(*foo == 0xFF,true);
}
TEST(DDKLentPtrTest,sharedPtrLentConstruction)
{
	ddk::shared_pointer_wrapper<DefaultType> fooShared = ddk::make_shared_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo = ddk::lend(fooShared);

	EXPECT_EQ(*foo == 0xFF,true);
}
TEST(DDKLentPtrTest,uniquePtrLentScope)
{
	ddk::unique_reference_counter refCounter;

	{
		ddk::unique_pointer_wrapper<DefaultType> fooUnique;

		{
			TestDynamicFactory<DefaultType> objFactory; 
			DefaultType* newNestedValue = objFactory.Allocate(0xFF);

			fooUnique = ddk::as_unique_reference(newNestedValue,ddk::tagged_pointer<ddk::unique_reference_counter>(&refCounter,ddk::ReferenceAllocationType::Embedded),nullptr);
			ddk::lent_pointer_wrapper<DefaultType> foo1 = ddk::lend(fooUnique);

			EXPECT_EQ(refCounter.hasStrongReferences(),true);
#if defined(DDK_DEBUG)
			EXPECT_EQ(refCounter.getNumLentReferences(),1);
#endif
			{
				ddk::lent_pointer_wrapper<DefaultType> foo2 = ddk::lend(fooUnique);

#if defined(DDK_DEBUG)
				EXPECT_EQ(refCounter.getNumLentReferences(),2);
#endif
				{
					ddk::lent_pointer_wrapper<DefaultType> foo3 = ddk::lend(fooUnique);

#if defined(DDK_DEBUG)
					EXPECT_EQ(refCounter.getNumLentReferences(),3);
#endif

					{
						ddk::lent_pointer_wrapper<DefaultType> foo4 = ddk::lend(fooUnique);

#if defined(DDK_DEBUG)
						EXPECT_EQ(refCounter.getNumLentReferences(),4);
#endif

						{
							ddk::lent_pointer_wrapper<DefaultType> foo5 = ddk::lend(fooUnique);

#if defined(DDK_DEBUG)
							EXPECT_EQ(refCounter.getNumLentReferences(),5);
#endif
						}

#if defined(DDK_DEBUG)
						EXPECT_EQ(refCounter.getNumLentReferences(),4);
#endif
					}

#if defined(DDK_DEBUG)
					EXPECT_EQ(refCounter.getNumLentReferences(),3);
#endif
				}

#if defined(DDK_DEBUG)
				EXPECT_EQ(refCounter.getNumLentReferences(),2);
#endif
			}

#if defined(DDK_DEBUG)
			EXPECT_EQ(refCounter.getNumLentReferences(),1);
#endif
		}

#if defined(DDK_DEBUG)
		EXPECT_EQ(refCounter.getNumLentReferences(),0);
#endif
	}

	EXPECT_EQ(refCounter.hasStrongReferences(),false);
}
TEST(DDKLentPtrTest,uniquePtrLentAssignment)
{
	ddk::unique_pointer_wrapper<DefaultType> fooUnique = ddk::make_unique_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);
}
TEST(DDKLentPtrTest,sharedPtrLentAssignment)
{
	ddk::shared_pointer_wrapper<DefaultType> fooShared = ddk::make_shared_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooShared);

	EXPECT_EQ(*foo,0xFF);
}
TEST(DDKLentPtrTest,pointerAccess)
{
	ddk::unique_pointer_wrapper<DefaultType> fooUnique = ddk::make_unique_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooUnique);

	EXPECT_EQ(foo->getValue(),0xFF);
}
TEST(DDKLentPtrTest,get)
{
	ddk::unique_pointer_wrapper<DefaultType> fooUnique = ddk::make_unique_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooUnique);

	EXPECT_EQ(ddk::get_raw_ptr(foo),fooUnique.get());
}
TEST(DDKLentPtrTest,clear)
{
	ddk::unique_pointer_wrapper<DefaultType> fooUnique = ddk::make_unique_reference<DefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);

	foo = nullptr;

	EXPECT_EQ(foo,nullptr);
}
TEST(DDKLentPtrTest,derivedConstruction)
{
	ddk::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ddk::make_unique_reference<DerivedDefaultType>(0xFF);
	const ddk::lent_pointer_wrapper<DefaultType> foo = ddk::lend(fooUnique);

	ddk::lent_pointer_wrapper<const DefaultType> foo2 = foo;

	EXPECT_EQ(*foo,0xFF);
}
TEST(DDKLentPtrTest,derivedAssignment)
{
	ddk::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ddk::make_unique_reference<DerivedDefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo;

	foo = ddk::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);
}
TEST(DDKLentPtrTest,dynamicCast)
{
	ddk::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ddk::make_unique_reference<DerivedDefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo = ddk::lend(fooUnique);

	ddk::lent_pointer_wrapper<DerivedDefaultType> fooDerived = ddk::dynamic_lent_cast<DerivedDefaultType>(foo);

	EXPECT_EQ(*fooDerived,0xFF);
}
TEST(DDKLentPtrTest,staticCast)
{
	ddk::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ddk::make_unique_reference<DerivedDefaultType>(0xFF);
	ddk::lent_pointer_wrapper<DefaultType> foo = ddk::lend(fooUnique);

	ddk::lent_pointer_wrapper<DerivedDefaultType> fooDerived = ddk::static_lent_cast<DerivedDefaultType>(foo);

	EXPECT_EQ(*fooDerived,0xFF);
}
