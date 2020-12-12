#include <gtest/gtest.h>
#include "ddk_inherited_value.h"
#include <utility>
#include <string>
#include "test_utils.h"

using namespace testing;
using testing::Types;

class DDKInheritedValuetTest : public Test
{
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::inherited_value<DefaultType> foo = ddk::make_inherited_value<DerivedDefaultType>(10);

}
