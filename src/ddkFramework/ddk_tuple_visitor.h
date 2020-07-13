#pragma once

#include <tuple>
#include <utility>

namespace ddk
{

template<typename Tuple>
struct tuple_visitor;

template<typename ... Types>
struct tuple_visitor<std::tuple<Types...>>
{
	static const size_t s_numTypes = sizeof...(Types);

public:
	tuple_visitor(const std::tuple<Types...>& i_tuple)
	: m_tuple(i_tuple)
	{
	}

	template<typename Visitor>
	void visit(const Visitor& i_visitor) const
	{
		static const size_t s_numTypes = sizeof...(Types);

		return _visit(std::make_integer_sequence<size_t,s_numTypes>(),i_visitor);
	}
	template<typename Visitor>
	void visit(Visitor& i_visitor) const
	{
		static const size_t s_numTypes = sizeof...(Types);

		return _visit(std::make_integer_sequence<size_t,s_numTypes>(),i_visitor);
	}

private:
	template<size_t ... Seq, typename Visitor>
	void _visit(const std::integer_sequence<size_t,Seq...>&, Visitor& i_visitor) const
	{
		typedef void(*visit_func_t)(Visitor&);
		static const visit_func_t visitFuncs[s_numTypes] = { &tuple_visitor<std::tuple<Types...>>::__visit<Seq,Visitor> ... };

		for(size_t typeIndex=0;typeIndex<s_numTypes;++typeIndex)
		{
			(*visitFuncs[typeIndex])(i_visitor);
		}
	}
	template<size_t Seq, typename Visitor>
	void __visit(Visitor& i_visitor)
	{
		i_visitor.visit(std::get<Seq>(m_tuple));
	}
	template<size_t ... Seq, typename Visitor>
	void _visit(const std::integer_sequence<size_t,Seq...>&, const Visitor& i_visitor) const
	{
		typedef void(*visit_func_t)(const Visitor&);
		static const size_t s_numTypes = sizeof...(Types);
		static const visit_func_t visitFuncs[] = { &tuple_visitor<std::tuple<Types...>>::__const_visit<Seq,Visitor> ... };

		for(size_t typeIndex=0;typeIndex<s_numTypes;++typeIndex)
		{
			(*visitFuncs[typeIndex])(i_visitor);
		}
	}
	template<size_t Seq, typename Visitor>
	void __const_visit(const Visitor& i_visitor)
	{
		i_visitor.visit(std::get<Seq>(m_tuple));
	}

	const std::tuple<Types...>& m_tuple;
};

}