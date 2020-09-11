#include <gtest/gtest.h>
#include "ddk_function_view.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include <map>
#include "ddk_function_ops.h"

using namespace testing;
using testing::Types;

class DDKFunctionTest : public Test
{
};

int sum_func(int a, float b)
{
	return a;
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
	Foo thisFoo;
	ddk::function<int(int,float)> res1 = ddk::make_function(&sum_func);
	ddk::function<size_t(char,const std::string&)> res2 = ddk::make_function(&thisFoo,&Foo::member_func);
	ddk::function<bool(const std::string&)> res3 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); });

	ddk::function<int(int,float)> res4 = ddk::make_function(&sum_func,ddk::system_allocator{});
	ddk::function<size_t(char,const std::string&)> res5 = ddk::make_function(&thisFoo,&Foo::member_func,ddk::system_allocator{});
	ddk::function<bool(const std::string&)> res6 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },ddk::system_allocator{});

	ddk::function<int(float)> res7 = ddk::make_function(&sum_func,1,ddk::arg_0);
	ddk::function<size_t(const std::string&)> res8 = ddk::make_function(&thisFoo,&Foo::member_func,'a',ddk::arg_0);
	ddk::function<bool()> res9 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },"hola");

	ddk::function<bool(const std::string&)> ress = res6 && res6;

	ddk::function<int(float)> res10 = ddk::make_function(&sum_func,ddk::system_allocator{},1,ddk::arg_0);
	ddk::function<size_t(const std::string&)> res11 = ddk::make_function(&thisFoo,&Foo::member_func,ddk::system_allocator{},'a',ddk::arg_0);
	ddk::function<bool()> res12 = ddk::make_function([](const std::string& i_str){ return i_str.empty(); },ddk::system_allocator{},"hola");

    ddk::function<int(int,std::string,float,double,char)> foo;

    ddk::function<int(float,std::string,double)> foo1 = foo(1,ddk::arg_1,ddk::arg_0,ddk::arg_2,'a');

    ddk::function<int(double,float)> foo2 = foo1(ddk::arg_1,"hola",ddk::arg_0);

    ddk::function<int(double,float,std::string,char,int)> foo3 = foo(ddk::arg_3,ddk::arg_2,ddk::arg_1,ddk::arg_4,ddk::arg_0);

	if (ddk::function_view<int(double, float, std::string, char, int)> foo3View = ddk::lend(foo3))
	{
		int result = foo3View(20.f,10.f,"hola",'a',3);
	}
}


