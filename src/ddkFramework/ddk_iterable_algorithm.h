#pragma once

#include "ddk_iteration.h"
#include "ddk_iterable_value.h"

namespace ddk
{
namespace algo
{

//template<typename Predicate>
//class find
//{
//	template<typename Iterable>
//	iteration<typename Iterable::traits> operator<<=(const find& i_lhs,const Iterable& i_iterable)
//	{
//		typedef typename Iterable::iterable_value iterable_value;
//
//		return make_function([&m_predicate](iterable_value i_value) { if(m_predicate(*i_value) { stop(erase_value(std::move(i_value))); })}) <<= i_iterable;
//	}
//
//public:
//	find(Predicate&& i_predicate);
//
//private:
//	Predicate m_predicate;
//};
//
//template<typename Predicate>
//class erase
//{
//	template<typename Iterable>
//	iteration operator<<=(const erase& i_lhs,const Iterable& i_iterable)
//	{
//		typedef typename Iterable::iterable_value iterable_value;
//
//		return make_function([&m_predicate](iterable_value i_value) { if(m_predicate(*i_value) { erase_value(std::move(i_value)); stop_iteration(); })}) <<= i_iterable;
//	}
//
//public:
//	erase(Predicate&& i_predicate);
//
//private:
//	Predicate m_predicate;
//};
//
//template<typename Predicate>
//class erase_any
//{
//	template<typename Iterable>
//	iteration operator<<=(const erase_any& i_lhs, const Iterable& i_iterable)
//	{
//		typedef typename Iterable::iterable_value iterable_value;
//
//		return make_function([&m_predicate](iterable_value i_value){ while(m_predicate(*i_value){ erase_value(std::move(i_value)); } )}) <<= i_iterable;
//	}
//
//public:
//	erase_any(Predicate&& i_predicate);
//
//private:
//	Predicate m_predicate;
//};

template<typename PredicateA, typename PredicateB>
class swap
{
	template<typename Iterable>
	friend inline iteration<typename Iterable::traits> operator<<=(const swap& i_lhs,const Iterable& i_iterable)
	{
		typedef typename Iterable::value_type value_type;
		typedef typename detail::intersection_iterable_transformed_traits<value_type,Iterable,Iterable>::iterable_value iterable_value;
		typedef typename Iterable::iterable_value iterable_value;

		return make_function([](iterable_value i_value){ std::swap(i_value->get<0>(),i_value->get<1>()); stop_iteration(); })  <<= (m_perdicateA <<= i_iterable) & (m_predicateB <<= i_iterable)
	}

public:
	template<typename PPredicateA, typename PPredicateB>
	swap(PPredicateA&& i_predicateA, PPredicateB&& i_predicateB);

private:
	const PredicateA m_predicateA;
	const PredicateB m_predicateB;
};

}
}

#include "ddk_iterable_algorithm.inl"