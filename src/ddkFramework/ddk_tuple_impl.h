//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_template_helper.h"
#include "ddk_embedded_type.h"
#include "ddk_type_concepts.h"
#include "ddk_rank_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<size_t Index,typename T>
class tuple_base
{
public:
    typedef typename embedded_type<T>::ref_type reference;
    typedef typename embedded_type<T>::cref_type const_reference;
    typedef typename embedded_type<T>::rref_type rreference;

    tuple_base() = default;
    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
    constexpr explicit tuple_base(Args&& ... i_args);
    constexpr reference get();
    constexpr rreference extract()&&;
    constexpr const_reference get() const;
    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
    constexpr bool set(Args&& ... i_args);

private:
    embedded_type<T> m_value;
};

template<typename,typename ...>
class tuple_impl;

template<>
class tuple_impl<mpl::sequence<>>
{
public:
    tuple_impl() = default;
};

template<typename Type>
class tuple_impl<mpl::sequence<0>,Type>
{
    template<typename,typename...>
    friend class tuple_impl;

public:
    constexpr tuple_impl() = default;
    TEMPLATE(size_t IIndex,typename Arg)
    REQUIRES(IS_SAME_RANK(IIndex,0),IS_CONSTRUCTIBLE(Type,Arg))
    constexpr tuple_impl(const mpl::sequence<IIndex>&,Arg&& i_arg);
    TEMPLATE(typename Arg)
    REQUIRES(IS_CONSTRUCTIBLE(Type,Arg))
    constexpr explicit tuple_impl(Arg&& i_val);
    template<typename TType>
    constexpr tuple_impl(const tuple_impl<mpl::sequence<0>,TType>& other);
    template<typename TType>
    constexpr tuple_impl(tuple_impl<mpl::sequence<0>,TType>&& other);
    template<size_t IIndex>
    constexpr typename embedded_type<Type>::cref_type get() const;
    template<size_t IIndex>
    constexpr typename embedded_type<Type>::ref_type get();
    template<size_t IIndex>
    constexpr typename embedded_type<Type>::rref_type extract()&&;
    template<size_t IIndex,typename Arg>
    constexpr typename embedded_type<Type>::ref_type set(Arg&& i_arg);
    constexpr tuple_impl<mpl::sequence<0>,Type>* operator->();
    constexpr const tuple_impl<mpl::sequence<0>,Type>* operator->() const;
    static constexpr size_t size();
    constexpr bool empty() const;

private:
    Type m_val;
};

template<size_t Index,typename ... Types>
using tuple_base_by_index = mpl::nth_type_of_t<Index,tuple_base<Index,Types>...>;

template<size_t ... Indexs,typename ... Types>
class tuple_impl<mpl::sequence<Indexs...>,Types...> : protected tuple_base<Indexs,Types> ...
{
    template<typename,typename...>
    friend class tuple_impl;
    template<size_t Index>
    using type_by_index = mpl::nth_type_of_t<Index,Types...>;

public:
    tuple_impl() = default;
    TEMPLATE(size_t ... IIndexs,typename ... TTypes)
    REQUIRES(IS_CONSTRUCTIBLE(type_by_index<IIndexs>,TTypes)...)
    constexpr tuple_impl(const mpl::sequence<IIndexs...>&,TTypes&& ... i_args);
    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Types,Args)...)
    constexpr explicit tuple_impl(Args&& ... i_args);
    constexpr tuple_impl(const tuple_impl& other);
    constexpr tuple_impl(tuple_impl&& other);
    template<typename ... TTypes>
    constexpr tuple_impl(const tuple_impl<mpl::sequence<Indexs...>,TTypes...>& other);
    template<typename ... TTypes>
    constexpr tuple_impl(tuple_impl<mpl::sequence<Indexs...>,TTypes...>&& other);
    constexpr tuple_impl& operator=(const tuple_impl& other) = default;
    constexpr tuple_impl& operator=(tuple_impl&& other) = default;
    template<size_t IIndex>
    constexpr typename tuple_base_by_index<IIndex,Types...>::const_reference get() const;
    template<size_t IIndex>
    constexpr typename tuple_base_by_index<IIndex,Types...>::reference get();
    template<size_t IIndex>
    constexpr typename tuple_base_by_index<IIndex,Types...>::rreference extract()&&;
    template<size_t ... IIndexs,typename ... Args>
    constexpr void set(const mpl::sequence<IIndexs...>&,Args&& ... i_args);
    template<typename ... Args>
    constexpr auto prepend(Args&& ... i_args) const;
    template<typename ... Args>
    constexpr auto append(Args&& ... i_args) const;
    template<size_t IIndex,typename Arg>
    constexpr typename tuple_base_by_index<IIndex,Types...>::reference set(Arg&& i_val);
    tuple_impl<mpl::sequence<Indexs...>,Types...>* operator->();
    constexpr const tuple_impl<mpl::sequence<Indexs...>,Types...>* operator->() const;
    static constexpr size_t size();
    constexpr bool empty() const;
};

}

template<typename ... Types>
using tuple = detail::tuple_impl<typename mpl::make_sequence<0,mpl::num_types<Types...>>::type,Types...>;

template<typename ... Types>
constexpr auto make_tuple(Types&& ... vals);
template<size_t ... Indexs,typename ... Types>
constexpr auto make_indexed_tuple(const mpl::sequence<Indexs...>&,Types&& ... vals);
template<typename ... TypesA,typename ... TypesB>
constexpr tuple<TypesA...,TypesB...> merge(const tuple<TypesA...>& i_lhs,const tuple<TypesB...>& i_rhs);
template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,typename ... TypesA,typename ... TypesB>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>& i_destSeq,const tuple<TypesA...>& i_lhs,const tuple<TypesB...>& i_rhs);
template<typename ... Types,typename ... Args>
constexpr tuple<Types...,Args...> merge_args(const tuple<Types...>& i_lhs,Args&& ... i_args);
template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,typename ... Types,typename ... Args>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>& i_destSeq,const tuple<Types...>& i_lhs,Args&& ... i_args);

}

#include "ddk_tuple_impl.inl"