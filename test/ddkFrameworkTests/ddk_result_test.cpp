#include <gtest/gtest.h>
#include "ddk_result.h"
#include "test_utils.h"

using namespace testing;
using testing::Types;

class DDKResultTest: public Test
{
};

enum class DefaultError
{
	Error
};

TEST(DDKResultTest,defaultConstruction)
{
}