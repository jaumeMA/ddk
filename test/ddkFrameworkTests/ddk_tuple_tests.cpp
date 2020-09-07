#include <gtest/gtest.h>
#include "ddk_tuple.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include <map>

using namespace testing;
using testing::Types;

class DDKTupleTest : public Test
{
};

TEST(DDKTupleTest,defaultConstruction)
{
    ddk::tuple<int,std::string,float> foo(10,"hola",0.5f);

	EXPECT_EQ(foo.get<0>(),10);
	EXPECT_EQ(foo.get<1>(),"hola");
	EXPECT_EQ(foo.get<2>(),0.5f);
}

