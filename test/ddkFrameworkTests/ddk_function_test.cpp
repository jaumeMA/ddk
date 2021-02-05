#include <gtest/gtest.h>
#include "ddk_function_view.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include <map>
#include "ddk_function_ops.h"
#include "ddk_unique_pointer_wrapper.h"

using namespace testing;
using testing::Types;

class DDKFunctionTest : public Test
{
};

int sum_func(int a, float b)
{
	return a;
}

int prod_func(int a, const std::string& b,float c,double d,char f)
{
    return a * static_cast<int>(b.size());
}

class Foo
{
public:
	size_t member_func(char a, const std::string& b)
	{
		return b.size();
	}
};

TEST(DDKFunctionTest,defaultConstruction)
{
    ddk::function<int(int,std::string,float,double,char)> foo;

    ddk::function<void(const ddk::unique_pointer_wrapper<int>&)> foo2 = ddk::make_function([](const ddk::unique_pointer_wrapper<int>& i_value){});

    ddk::unique_pointer_wrapper<int> kk;

    foo2(std::move(kk));
}
TEST(DDKFunctionTest,funcMakeConstruction)
{
	Foo thisFoo;

	ddk::function<int(int,float)> res1 = ddk::make_function(&sum_func);
	ddk::function<size_t(char,const std::string&)> res2 = ddk::make_function(&thisFoo,&Foo::member_func);
	ddk::function<bool(const std::string&)> res3 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); });

	ddk::function<int(int,float)> res4 = ddk::make_function(&sum_func,ddk::system_allocator{});
	ddk::function<size_t(char,const std::string&)> res5 = ddk::make_function(&thisFoo,&Foo::member_func,ddk::system_allocator{});
	ddk::function<bool(const std::string&)> res6 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },ddk::system_allocator{});
}

TEST(DDKFunctionTest,funcMakeSpecialization)
{
	Foo thisFoo;

	ddk::function<int(float)> res7 = ddk::make_function(&sum_func,1,ddk::arg_0);
	ddk::function<size_t(const std::string&)> res8 = ddk::make_function(&thisFoo,&Foo::member_func,'a',ddk::arg_0);
	ddk::function<bool()> res9 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },"hola");

	ddk::function<int(float)> res10 = ddk::make_function(&sum_func,ddk::system_allocator{},1,ddk::arg_0);
	ddk::function<size_t(const std::string&)> res11 = ddk::make_function(&thisFoo,&Foo::member_func,ddk::system_allocator{},'a',ddk::arg_0);
	ddk::function<bool()> res12 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },ddk::system_allocator{},"hola");
}
TEST(DDKFunctionTest,funcSpecialization)
{
    ddk::function<int(int,const std::string&,float,double,char)> foo = ddk::make_function(&prod_func);;

    ddk::function<int(float,const std::string&,double)> foo1 = foo(1,ddk::arg_1,ddk::arg_0,ddk::arg_2,'a');

    ddk::function<int(double,float)> foo2 = foo1(ddk::arg_1,"hola",ddk::arg_0);

    ddk::function<int(double,float,const std::string&,char,int)> foo3 = foo(ddk::arg_3,ddk::arg_2,ddk::arg_1,ddk::arg_4,ddk::arg_0);
}
TEST(DDKFunctionTest,funcOps)
{
    ddk::function<bool(double,float)> foo1;

    ddk::function<bool(double,float)> foo2;

	ddk::function<bool(double,float)> foo12 = foo1 && foo2;
}
TEST(DDKFunctionTest,funcView)
{
    ddk::function<int(int,const std::string&,float,double,char)> foo = ddk::make_function(&prod_func);

    ddk::function<int(double,float,const std::string&,char,int)> foo3 = foo(ddk::arg_3,ddk::arg_2,ddk::arg_1,ddk::arg_4,ddk::arg_0);

	if (ddk::function_view<int(double, float, const std::string&, char, int)> foo3View = ddk::lend(foo3))
	{
		int result = foo3View(20.f,10.f,"hola",'a',3);

        EXPECT_EQ(result,12);
	}
}
TEST(DDKFunctionTest,funcComposition)
{
    ddk::function<bool(int)> foo1;
    ddk::function<int(char)> foo2;
    ddk::function<char(float,double)> foo3;

    ddk::function<bool(float,double)> foo12 =  foo1 <<= foo2 <<= foo3;
}
TEST(DDKFunctionTest,funcIntersection)
{
    ddk::function<bool(int,char,float)> foo1;
    ddk::function<int(float,double)> foo2;
    ddk::function<char(float,double)> foo3;
    ddk::function<float(float,double)> foo4;

    ddk::function<bool(float,double)> foo12 =  foo1 <<= ddk::fusion(foo2,foo3,foo4);
}
TEST(DDKFunctionTest,funcUnion)
{
    ddk::function<bool(int,char,float)> foo1;
    ddk::function<int(float,double)> foo2;
    ddk::function<char(const std::string&)> foo3;
    ddk::function<float(char)> foo4;

    ddk::function<bool(float,double,const std::string&,char)> foo12 =  foo1 <<= ddk::concat(foo2,foo3,foo4);
}
