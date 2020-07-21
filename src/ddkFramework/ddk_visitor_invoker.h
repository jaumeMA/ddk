#pragma once

#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Return, typename ... Types>
class variant_visitor_invoker
{
    static const size_t _numTypes = mpl::get_num_types<Types...>::value;

public:
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		return _visitor.template visit<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		return _visitor.template visit<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		return _visitor.template visit<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		return _visitor.template visit<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(visitor& _visitor, variant_impl<Types...>&& var)
	{
		return _visitor.template visit<TypePos>(var.template extract<TypePos>());
	}
    template<typename visitor, size_t TypePos>
    inline static Return inner_invoke(const visitor& _visitor, variant_impl<Types...>&& var)
	{
		return _visitor.template visit<TypePos>(var.template extract<TypePos>());
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		return _visitor.visit(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		return _visitor.visit(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		return _visitor.visit(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		return _visitor.visit(std::forward<ref_type>(var.template get<TypePos>()));
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(visitor& _visitor, variant_impl<Types...>&& var)
	{
		return _visitor.visit(var.template extract<TypePos>());
	}
    template<typename visitor, size_t TypePos>
    inline static Return outer_invoke(const visitor& _visitor, variant_impl<Types...>&& var)
	{
		return _visitor.visit(var.template extract<TypePos>());
	}

public:
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, std::move(var));
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, std::move(var));
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor, TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor, TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, var);
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, std::move(var));
	}
    template<typename visitor, size_t ... TypeIndexs>
    inline static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		static const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		return (*funcTable[var.m_currentType])(_visitor, std::move(var));
	}
};

}
}
