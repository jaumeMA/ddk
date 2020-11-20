#pragma once

#include "ddk_iteration.h"
#include "ddk_iterable_value.h"

namespace ddk
{
namespace algo
{

template<typename Predicate>
class erase_any
{
	template<typename Iterable>
	iteration operator<<=(const erase& i_lhs,const Iterable& i_iterable)
	{
		typedef typename Iterable::iterable_value iterable_value;

		return make_function([&m_predicate](iterable_value i_value) { if(m_predicate(*i_value) { stop(erase_value(std::move(i_value))); })}) <<= i_iterable;
	}

public:
	erase(Predicate&& i_predicate);

private:
	Predicate m_predicate;
};

template<typename Predicate>
class erase_any
{
	template<typename Iterable>
	iteration operator<<=(const erase& i_lhs, const Iterable& i_iterable)
	{
		typedef typename Iterable::iterable_value iterable_value;

		return make_function([&m_predicate](iterable_value i_value){ while(m_predicate(*i_value){ i_value = erase_value(std::move(i_value)); } )}) <<= i_iterable;
	}

public:
	erase(Predicate&& i_predicate);

private:
	Predicate m_predicate;
};

}
}