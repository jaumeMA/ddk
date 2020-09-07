#include <gtest/gtest.h>
#include "ddk_function.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include <map>

using namespace testing;
using testing::Types;

class DDKFunctionTest : public Test
{
};

TEST(DDKFunctionTest,defaultConstruction)
{
    ddk::function<int(int,std::string,float,double,char)> foo;

    ddk::function<int(float,std::string,double)> foo1 = foo(1,ddk::arg_1,ddk::arg_0,ddk::arg_2,'a');

    ddk::function<int(double,float)> foo2 = foo1(ddk::arg_1,"hola",ddk::arg_0);
}


