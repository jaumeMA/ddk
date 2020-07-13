#include <gtest/gtest.h>
#include "ewas_flagset.h"
#include "ewas_scoped_enum.h"

using namespace testing;
using testing::Types;

class EwasFlagSetTest : public Test
{
public:
	SCOPED_ENUM_DECL(Type,
						A,
						B,
						C,
						D
					);
};

TEST(EwasFlagSetTest,defaultConstruction)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);
}
TEST(EwasFlagSetTest,construction)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet(EwasFlagSetTest::Type::A);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,assignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet.set(EwasFlagSetTest::Type::A);

	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,multipleConstruction)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);
}
TEST(EwasFlagSetTest,multipleassignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet = EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::D;

	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);
}
TEST(EwasFlagSetTest,multipleconsecutiveassignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet;

	EXPECT_EQ(flagSet.empty(),true);

	flagSet.set(EwasFlagSetTest::Type::A);
	flagSet.set(EwasFlagSetTest::Type::B);
	flagSet.set(EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);
}
TEST(EwasFlagSetTest,multipleConstructionSomeReset)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);

	flagSet.reset(EwasFlagSetTest::Type::B);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);
}
TEST(EwasFlagSetTest,multipleConstructionReset)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet.empty(),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),true);

	flagSet.reset();

	EXPECT_EQ(flagSet.empty(),true);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,orOperator)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet3 = flagSet1 | flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,orOperatorAssignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),false);

	flagSet1 |= flagSet2;

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,andOperator)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),true);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet3 = flagSet1 & flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,andOperatorAssignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),true);

	flagSet2 &= flagSet1;

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),false);
}
TEST(EwasFlagSetTest,exclOperator)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),true);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet3 = flagSet1 ^ flagSet2;

	EXPECT_EQ(flagSet3.empty(),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet3.test(EwasFlagSetTest::Type::D),true);
}
TEST(EwasFlagSetTest,exclOperatorAssignment)
{
	ewas::flag_set<EwasFlagSetTest::Type> flagSet1(EwasFlagSetTest::Type::A | EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C);

	EXPECT_EQ(flagSet1.empty(),false);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet1.test(EwasFlagSetTest::Type::D),false);

	ewas::flag_set<EwasFlagSetTest::Type> flagSet2(EwasFlagSetTest::Type::B | EwasFlagSetTest::Type::C | EwasFlagSetTest::Type::D);

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),true);

	flagSet2 ^= flagSet1;

	EXPECT_EQ(flagSet2.empty(),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::A),true);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::B),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::C),false);
	EXPECT_EQ(flagSet2.test(EwasFlagSetTest::Type::D),true);
}