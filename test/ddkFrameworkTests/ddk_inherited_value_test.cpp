#include <gtest/gtest.h>
#include "ddk_inherited_value.h"
#include <utility>
#include <string>
#include "test_utils.h"
#include "ddk_rtti.h"
#include "ddk_any_value.h"
#include "ddk_dynamic_callable.h"

using namespace testing;
using testing::Types;

namespace prova
{


class BaseType1
{
public:
	BaseType1(int i_value = 0)
	: m_value(i_value)
	{
	}
	virtual ~BaseType1() = default;

private:
	int m_value;
};

class BaseType2
{
public:
	BaseType2(int i_value = 0)
	: m_value(i_value)
	{
	}
	virtual ~BaseType2() = default;

private:
	int m_value;
};

class DerivedBaseType1 : public BaseType1, public BaseType2
{
public:
	DerivedBaseType1(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType1,BaseType1,BaseType2);

class DerivedBaseType2: public BaseType1, public BaseType2
{
public:
	DerivedBaseType2(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType2,BaseType1,BaseType2);

class DerivedBaseType3: public BaseType1,public BaseType2
{
public:
	DerivedBaseType3(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType3,BaseType1,BaseType2);

class DerivedBaseType4: public BaseType1,public BaseType2
{
public:
	DerivedBaseType4(int i_value = 0)
	: BaseType1(i_value)
	,BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType4,BaseType1,BaseType2);

class DerivedBaseType5: public BaseType1,public BaseType2
{
public:
	DerivedBaseType5(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType5,BaseType1,BaseType2);

class DerivedBaseType6: public BaseType1,public BaseType2
{
public:
	DerivedBaseType6(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType6,BaseType1,BaseType2);

class DerivedBaseType7: public BaseType1,public BaseType2
{
public:
	DerivedBaseType7(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType7,BaseType1,BaseType2);

class DerivedBaseType8: public BaseType1,public BaseType2
{
public:
	DerivedBaseType8(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType8,BaseType1,BaseType2);

class DerivedBaseType9: public BaseType1,public BaseType2
{
public:
	DerivedBaseType9(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType9,BaseType1,BaseType2);

class DerivedBaseType10: public BaseType1,public BaseType2
{
public:
	DerivedBaseType10(int i_value = 0)
	: BaseType1(i_value)
	, BaseType2(i_value)
	{
	}

} PUBLISH_RTTI_INHERITANCE(DerivedBaseType10,BaseType1,BaseType2);

}

class DDKInheritedValuetTest : public Test
{
};

struct DerivedTypeMultiVisitor
{
	typedef int return_type;

	return_type operator()(const prova::DerivedBaseType1&, prova::DerivedBaseType1&) const
	{
		return 1;
	}
	return_type operator()(prova::DerivedBaseType1&, prova::DerivedBaseType2&) const
	{
		return 1;
	}
	template<typename T1, typename T2>
	return_type operator()(T1& i_lhs, T2& i_rhs) const
	{
	    return 0;
	}
};

TEST(DDKInheritedValuetTest,defaultConstruction)
{
	ddk::unique_object<prova::BaseType1> kk_uniq = ddk::make_unique_value<prova::DerivedBaseType1>(10);
	ddk::distributed_object<prova::BaseType1> kk_dist = ddk::make_distributed_value<prova::DerivedBaseType1>(10);

	ddk::lent_object<const prova::BaseType1> kkLentUniq = ddk::lend(kk_uniq);
	ddk::lent_object<prova::BaseType1> kkLentDist = ddk::lend(kk_dist);
	ddk::lent_value<prova::BaseType1> kkProm = ddk::promote_to_value(kkLentDist);

	ddk::distributed_value<prova::BaseType1> foo0 = ddk::make_distributed_value<prova::DerivedBaseType1>(10);
	ddk::distributed_value<prova::BaseType1> foo1 = ddk::make_distributed_value<prova::DerivedBaseType2>(20);
	const ddk::distributed_value<prova::BaseType1> foo2 = ddk::make_distributed_value<prova::DerivedBaseType1>(10);
	const ddk::distributed_value<prova::BaseType1> foo3 = ddk::make_distributed_value<prova::DerivedBaseType2>(20);
	const DerivedTypeMultiVisitor multiVisitor;

	//ddk::dynamic_callable<bool,prova::BaseType1>([](auto&& i_value){ return true; });


	//int a = 0;

	ddk::visit<DerivedTypeMultiVisitor>(kkLentUniq,kkLentDist);
	ddk::visit(multiVisitor,foo2,foo3);
	ddk::visit([](auto&& i_lhs, auto&& i_rhs){ return 0; },foo0,foo1);
	//int b = 0;
}
