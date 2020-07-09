#include <gtest/gtest.h>
#include "lent_pointer_wrapper.h"
#include "unique_pointer_wrapper.h"
#include "shared_pointer_wrapper.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "reference_wrapper.h"

using namespace testing;
using testing::Types;

class EwasUniquePtrtTest : public Test
{
};

TEST(EwasLentPtrTest,defaultConstruction)
{
	ewas::lent_pointer_wrapper<DefaultType> foo;

	EXPECT_EQ(foo==nullptr,true);
}
TEST(EwasLentPtrTest,uniquePtrLentConstruction)
{
	ewas::unique_pointer_wrapper<DefaultType> fooUnique = ewas::make_unique_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo = ewas::lend(fooUnique);

	EXPECT_EQ(*foo == 0xFF,true);
}
TEST(EwasLentPtrTest,sharedPtrLentConstruction)
{
	ewas::shared_pointer_wrapper<DefaultType> fooShared = ewas::make_shared_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo = ewas::lend(fooShared);

	EXPECT_EQ(*foo == 0xFF,true);
}
TEST(EwasLentPtrTest,uniquePtrLentScope)
{
	ewas::unique_reference_counter refCounter;

	{
		ewas::unique_pointer_wrapper<DefaultType> fooUnique;

		{
			TestDynamicFactory<DefaultType> objFactory; 
			DefaultType* newNestedValue = objFactory.Allocate(0xFF);

			fooUnique = ewas::as_unique_reference(newNestedValue,tagged_pointer<ewas::unique_reference_counter>(&refCounter,ewas::ReferenceAllocationType::Embedded));
			ewas::lent_pointer_wrapper<DefaultType> foo1 = ewas::lend(fooUnique);

			EXPECT_EQ(refCounter.hasStrongReferences(),true);
#if defined(EWAS_DEBUG)
			EXPECT_EQ(refCounter.getNumWeakReferences(),1);
#endif
			{
				ewas::lent_pointer_wrapper<DefaultType> foo2 = ewas::lend(fooUnique);

#if defined(EWAS_DEBUG)
				EXPECT_EQ(refCounter.getNumWeakReferences(),2);
#endif
				{
					ewas::lent_pointer_wrapper<DefaultType> foo3 = ewas::lend(fooUnique);

#if defined(EWAS_DEBUG)
					EXPECT_EQ(refCounter.getNumWeakReferences(),3);
#endif

					{
						ewas::lent_pointer_wrapper<DefaultType> foo4 = ewas::lend(fooUnique);

#if defined(EWAS_DEBUG)
						EXPECT_EQ(refCounter.getNumWeakReferences(),4);
#endif

						{
							ewas::lent_pointer_wrapper<DefaultType> foo5 = ewas::lend(fooUnique);

#if defined(EWAS_DEBUG)
							EXPECT_EQ(refCounter.getNumWeakReferences(),5);
#endif
						}

#if defined(EWAS_DEBUG)
						EXPECT_EQ(refCounter.getNumWeakReferences(),4);
#endif
					}

#if defined(EWAS_DEBUG)
					EXPECT_EQ(refCounter.getNumWeakReferences(),3);
#endif
				}

#if defined(EWAS_DEBUG)
				EXPECT_EQ(refCounter.getNumWeakReferences(),2);
#endif
			}

#if defined(EWAS_DEBUG)
			EXPECT_EQ(refCounter.getNumWeakReferences(),1);
#endif
		}

#if defined(EWAS_DEBUG)
		EXPECT_EQ(refCounter.getNumWeakReferences(),0);
#endif
	}

	EXPECT_EQ(refCounter.hasStrongReferences(),false);
}
TEST(EwasLentPtrTest,uniquePtrLentAssignment)
{
	ewas::unique_pointer_wrapper<DefaultType> fooUnique = ewas::make_unique_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);
}
TEST(EwasLentPtrTest,sharedPtrLentAssignment)
{
	ewas::shared_pointer_wrapper<DefaultType> fooShared = ewas::make_shared_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooShared);

	EXPECT_EQ(*foo,0xFF);
}
TEST(EwasLentPtrTest,pointerAccess)
{
	ewas::unique_pointer_wrapper<DefaultType> fooUnique = ewas::make_unique_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooUnique);

	EXPECT_EQ(foo->getValue(),0xFF);
}
TEST(EwasLentPtrTest,get)
{
	ewas::unique_pointer_wrapper<DefaultType> fooUnique = ewas::make_unique_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooUnique);

	EXPECT_EQ(ewas::get_raw_ptr(foo),fooUnique.get());
}
TEST(EwasLentPtrTest,clear)
{
	ewas::unique_pointer_wrapper<DefaultType> fooUnique = ewas::make_unique_reference<DefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);

	foo = nullptr;

	EXPECT_EQ(foo,nullptr);
}
TEST(EwasLentPtrTest,derivedConstruction)
{
	ewas::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ewas::make_unique_reference<DerivedDefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo = ewas::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);
}
TEST(EwasLentPtrTest,derivedAssignment)
{
	ewas::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ewas::make_unique_reference<DerivedDefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo;

	foo = ewas::lend(fooUnique);

	EXPECT_EQ(*foo,0xFF);
}
TEST(EwasLentPtrTest,dynamicCast)
{
	ewas::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ewas::make_unique_reference<DerivedDefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo = ewas::lend(fooUnique);

	ewas::lent_pointer_wrapper<DerivedDefaultType> fooDerived = ewas::dynamic_lent_cast<DerivedDefaultType>(foo);

	EXPECT_EQ(*fooDerived,0xFF);
}
TEST(EwasLentPtrTest,staticCast)
{
	ewas::unique_pointer_wrapper<DerivedDefaultType> fooUnique = ewas::make_unique_reference<DerivedDefaultType>(0xFF);
	ewas::lent_pointer_wrapper<DefaultType> foo = ewas::lend(fooUnique);

	ewas::lent_pointer_wrapper<DerivedDefaultType> fooDerived = ewas::static_lent_cast<DerivedDefaultType>(foo);

	EXPECT_EQ(*fooDerived,0xFF);
}
