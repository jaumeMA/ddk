#include <gtest/gtest.h>
#include "ddk_flagset.h"
#include "ddk_scoped_enum.h"

using namespace testing;
using testing::Types;

class DDKFlagSetTest : public Test
{
public:
	SCOPED_ENUM_DECL(Type,
						A,
						B,
						C,
						D
					);
};

TEST(DDKFlagSetTest,defaultConstruction)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);
}
TEST(DDKFlagSetTest,construction)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet(DDKFlagSetTest::Type::A);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,assignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet.set(DDKFlagSetTest::Type::A);

	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,multipleConstruction)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);
}
TEST(DDKFlagSetTest,multipleassignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet = DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::D;

	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);
}
TEST(DDKFlagSetTest,multipleconsecutiveassignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet.set(DDKFlagSetTest::Type::A);
	flagSet.set(DDKFlagSetTest::Type::B);
	flagSet.set(DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);
}
TEST(DDKFlagSetTest,multipleConstructionSomeReset)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);

	flagSet.reset(DDKFlagSetTest::Type::B);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);
}
TEST(DDKFlagSetTest,multipleConstructionReset)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),true);

	flagSet.reset();

	EXPECT_EQ(flagSet.empty(),true);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,orOperator)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet3 = flagSet1 | flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,orOperatorAssignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),false);

	flagSet1 |= flagSet2;

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,andOperator)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),true);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet3 = flagSet1 & flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,andOperatorAssignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),true);

	flagSet2 &= flagSet1;

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),false);
}
TEST(DDKFlagSetTest,exclOperator)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),true);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet3 = flagSet1 ^ flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet3.test(DDKFlagSetTest::Type::D),true);
}
TEST(DDKFlagSetTest,exclOperatorAssignment)
{
	ddk::flag_set<DDKFlagSetTest::Type> flagSet1(DDKFlagSetTest::Type::A | DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(DDKFlagSetTest::Type::D),false);

	ddk::flag_set<DDKFlagSetTest::Type> flagSet2(DDKFlagSetTest::Type::B | DDKFlagSetTest::Type::C | DDKFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),true);

	flagSet2 ^= flagSet1;

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet2.test(DDKFlagSetTest::Type::D),true);
}