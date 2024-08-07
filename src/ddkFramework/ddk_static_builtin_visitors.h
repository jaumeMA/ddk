//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_static_visitor.h"
#include "ddk_macros.h"
#include "ddk_crash.h"

namespace ddk
{
namespace detail
{

template<typename Storage, typename ... Types>
struct constructor_visitor : public static_visitor<Storage>
{
    typedef constructor_visitor t_visitor;

	constexpr constructor_visitor() = default;
	constexpr constructor_visitor(Storage& i_storage)
	{
	}
    template<size_t PosType, typename Type>
	constexpr Storage operator()(Type&& i_value) const
	{
		static_assert(PosType >= 0 && PosType < mpl::get_num_types<Types...>(),"Type out of bounds!");

		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		return { mpl::class_holder<varType>{},std::forward<Type>(i_value) };
	}
};

template<typename Storage, typename ... Types>
struct constructor_inplace_visitor : public static_visitor<void>
{
	typedef constructor_inplace_visitor t_visitor;

	constexpr constructor_inplace_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
	template<size_t PosType, typename ... Args>
	constexpr mpl::nth_type_of_t<PosType,Types...>& operator()(Args&& ... i_args) const
	{
		typedef typename mpl::nth_type_of<PosType, Types...>::type varType;

		return m_storage.template construct<varType>(std::forward<Args>(i_args)...);
	}

private:
	Storage& m_storage;
};

template<typename Storage, typename ... Types>
struct destructor_visitor : public static_visitor<void>
{
    typedef destructor_visitor t_visitor;

	constexpr destructor_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
    template<size_t PosType, typename Type>
	constexpr detail::void_t operator()(Type&&) const
	{
		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		m_storage.template destroy<varType>();

		return _void;
	}
    template<size_t PosType>
	constexpr void destroy()
	{
		static_assert(PosType >= 0 && PosType < mpl::get_num_types<Types...>(), "Type out of bounds!");

		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		m_storage.template destroy<varType>();
	}

private:
	Storage& m_storage;
};

template<typename Storage, typename ... Types>
struct assigner_visitor : public static_visitor<void>
{
    typedef assigner_visitor t_visitor;

	constexpr assigner_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
    template<size_t PosType, typename ... Args>
	constexpr mpl::nth_type_of_t<PosType,Types...>& operator()(Args&& ... i_args) const
	{
		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		return m_storage.template assign<varType>(std::forward<Args>(i_args)...);
	}

private:
	Storage& m_storage;
};

template<typename retType, typename ... Types>
struct val_retriever_visitor : public static_visitor<typename embedded_type<retType>::ref_type>
{
    typedef val_retriever_visitor t_visitor;
    typedef typename std::remove_const<typename embedded_type<retType>::internal_type>::type rawType;

	template<size_t PosType>
	constexpr typename embedded_type<retType>::ref_type operator()(typename embedded_type<retType>::ref_type val)
	{
		typedef typename embedded_type<retType>::ref_type ref_type;

		return std::forward<ref_type>(val);
	}
	template<size_t PosType>
	constexpr typename embedded_type<retType>::ref_type operator()(typename embedded_type<retType>::cref_type val) const
	{
		typedef typename embedded_type<retType>::ref_type ref_type;

		return const_cast<ref_type>(val);
	}
    //for the rest of unsused types
    template<size_t PosType, typename Type = void>
	constexpr typename embedded_type<retType>::ref_type operator()(...) const
	{
		typedef typename embedded_type<retType>::ref_type ref_type;

		DDK_FAIL("You shouldnt reach this point!");

		return ddk::crash_on_return<ref_type>::value();
	}
};

template<typename T,typename ... Types>
struct is_base_of_visitor : public static_visitor<bool>
{
	typedef is_base_of_visitor t_visitor;

	template<size_t PosType,typename Type>
	constexpr bool operator()(Type&& val,...) const
	{
		return std::is_base_of<T,typename mpl::nth_type_of<PosType,Types...>::type>::value;
	}
};

template<typename Variant, typename ... Types>
struct swaper_visitor : public static_visitor<void>
{
    typedef swaper_visitor t_visitor;

	constexpr swaper_visitor(Variant& _thisVariant, Variant& _otherVariant)
	: m_thisVariant(_thisVariant)
	, m_otherVariant(_otherVariant)
	{
	}
    template<size_t PosType, typename Type>
	constexpr detail::void_t operator()(Type&& otherVal)
	{
		static_assert(mpl::is_among_constructible_types<Type, Types...>, "Not present type!");

		m_thisVariant.m_storage.template swap<Type>(m_otherVariant.m_storage);

		return _void;
	}

private:
    Variant& m_thisVariant;
    Variant& m_otherVariant;
};

template<typename Variant, typename ... Types>
struct comparison_visitor : public static_visitor<bool>
{
    typedef comparison_visitor t_visitor;

	constexpr comparison_visitor(const Variant& _variant)
	: static_visitor<bool>()
	, m_variant(_variant)
	{
	}
    template<size_t PosType, typename Type>
	constexpr bool operator()(Type&& otherVal)
	{
		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		const val_retriever_visitor<varType,Types...> getter;

		//this reference here is ok by reference collapsing
		typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type seq_t;
		typedef variant_visitor_invoker<varType,Types...> variant_visitor_t;

		return variant_visitor_t::template inner_invoker(seq_t{},getter,m_variant) == otherVal;
	}

private:
    const Variant& m_variant;
};

}
}
