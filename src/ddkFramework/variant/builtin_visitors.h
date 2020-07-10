#pragma once

#include "ewas_static_visitor.h"
#include "ewas_macros.h"
#include "ewas_crash.h"

namespace ewas
{
namespace detail
{

template<typename Storage, typename ... Types>
struct constructor_visitor : public static_visitor<void>
{
    typedef constructor_visitor t_visitor;

    template<typename Type>
    struct _constructor;

    template<typename Type>
    struct _constructor
    {
        template<typename TType>
        static void construct(Storage& i_storage, TType&& val)
		{
			static_assert(mpl::is_among_convertible_types<Type, Types...>::value, "Constructing type non present in variant types!");

			i_storage.template construct<Type>(std::forward<TType>(val));
		}
    };

    constructor_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
    template<size_t PosType, typename Type>
    static detail::void_t construct(Storage& i_storage, Type&& val)
	{
		static_assert(PosType >= 0 && PosType < mpl::get_num_types<Types...>::value, "Type out of bounds!");

		typedef typename mpl::nth_type_of<PosType, Types...>::type TType;

		_constructor<TType>::template construct<Type>(i_storage, std::forward<Type>(val));

		return _void;
	}
    template<size_t PosType, typename Type>
    detail::void_t visit(Type&& i_value)
	{
		static_assert(mpl::is_among_convertible_types<Type, Types...>::value, "Not present type!");

		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		m_storage.template construct<varType>(std::forward<Type>(i_value));

		return _void;
	}

private:
	Storage& m_storage;
};

template<typename Storage, typename ... Types>
struct destructor_visitor : public static_visitor<void>
{
    typedef destructor_visitor t_visitor;

    destructor_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
    template<size_t PosType, typename Type>
    detail::void_t visit(Type&&)
	{
		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		m_storage.template destroy<varType>();

		return _void;
	}
    template<size_t PosType>
    void destroy()
	{
		static_assert(PosType >= 0 && PosType < mpl::get_num_types<Types...>::value, "Type out of bounds!");

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

    template<typename Type>
    struct _assigner
    {
        template<typename TType>
        static void assign(Storage& i_storage, TType&& val)
		{
			static_assert(mpl::is_among_convertible_types<Type, Types...>::value, "Assigning type non present in variant types!");

			i_storage.template assign<Type>(std::forward<TType>(val));
		}
    };

    assigner_visitor(Storage& i_storage)
	: m_storage(i_storage)
	{
	}
    template<size_t PosType, typename Type>
    static void assign(Storage& i_storage, Type&& val)
	{
		typedef typename mpl::nth_type_of<PosType, Types...>::type TType;

		_assigner<TType>::template assign<Type>(i_storage, std::forward<Type>(val));
	}
    template<size_t PosType, typename Type>
    detail::void_t visit(Type&& i_value)
	{
		static_assert(mpl::is_among_convertible_types<Type, Types...>::value, "Not present type!");

		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;

		m_storage.template assign<varType>(std::forward<Type>(i_value));

		return _void;
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
    typename embedded_type<retType>::ref_type visit(typename embedded_type<retType>::ref_type val)
	{
		typedef typename embedded_type<retType>::ref_type ref_type;

		return std::forward<ref_type>(val);
	}
	template<size_t PosType>
    typename embedded_type<retType>::cref_type visit(typename embedded_type<retType>::cref_type val) const
	{
		typedef typename embedded_type<retType>::cref_type ref_type;

		return std::forward<ref_type>(val);
	}

    //for the rest of unsused types
    template<size_t PosType, typename Type>
    typename embedded_type<retType>::ref_type visit(Type&& val, ...) const
	{
		typedef typename embedded_type<retType>::ref_type ref_type;

		EWAS_FAIL("You shouldnt reach this point!");

		return ewas::crash_on_return<ref_type>::value();
	}
};

template<typename Variant, typename ... Types>
struct swaper_visitor : public static_visitor<void>
{
    typedef swaper_visitor t_visitor;

    swaper_visitor(Variant& _thisVariant, Variant& _otherVariant)
	: m_thisVariant(_thisVariant)
	, m_otherVariant(_otherVariant)
	{
	}
    template<size_t PosType, typename Type>
    detail::void_t visit(Type&& otherVal)
	{
		static_assert(mpl::is_among_convertible_types<Type, Types...>::value, "Not present type!");

		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;
		typedef typename embedded_type<varType>::ref_type ref_type;

		val_retriever_visitor<Types...,ref_type> getter;

		//this reference here is ok by reference collapsing
		typedef typename mpl::create_range_rank<0,mpl::get_num_types<Types...>::value>::type range_seq_t;
		typedef variant_visitor_invoker<decltype(getter),Types...> variant_visitor_t;

		ref_type thisVal = variant_visitor_t::template invoke(range_seq_t{},getter,m_thisVariant);

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

    comparison_visitor(const Variant& _variant)
	: static_visitor<bool>()
	, m_variant(_variant)
	{
	}
    template<size_t PosType, typename Type>
    bool visit(Type&& otherVal)
	{
		typedef typename mpl::nth_type_of<PosType,Types...>::type varType;
		typedef typename embedded_type<varType>::cref_type cref_type;

		const val_retriever_visitor<cref_type> getter;

		//this reference here is ok by reference collapsing
		typedef typename mpl::create_range_rank<0,mpl::get_num_types<Types...>::value>::type range_seq_t;
		typedef variant_visitor_invoker<decltype(getter),Types...> variant_visitor_t;

		cref_type thisVal = variant_visitor_t::template invoke(range_seq_t{},getter,m_variant); \

		return thisVal == std::forward<Type>(otherVal);
	}

private:
    const Variant& m_variant;
};

}
}
