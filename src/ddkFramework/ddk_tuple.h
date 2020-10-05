#pragma once

#include "ddk_embedded_type.h"
#include "ddk_template_helper.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

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
    tuple_impl() = default;
    template<size_t IIndex, typename Arg>
    tuple_impl(const mpl::sequence<IIndex>&, Arg&& i_arg);
    template<typename Arg>
    tuple_impl(Arg&& i_val);
    template<typename TType>
    tuple_impl(const tuple_impl<mpl::sequence<0>,TType>& other);
    template<typename TType>
    tuple_impl(tuple_impl<mpl::sequence<0>,TType>&& other);
    template<size_t IIndex>
    typename embedded_type<Type>::cref_type get() const;
    template<size_t IIndex>
    typename embedded_type<Type>::ref_type get();
    template<size_t IIndex, typename Arg>
    void set(Arg&& i_arg);
    static constexpr size_t size();

private:
	Type m_val;
};

template<size_t Index1, size_t Index2, size_t ... Indexs,typename Type1, typename Type2, typename ... Types>
class tuple_impl<mpl::sequence<Index1,Index2,Indexs...>,Type1,Type2,Types...>
{
    template<typename,typename...>
    friend class tuple_impl;

public:
    tuple_impl();
    template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename Arg1, typename Arg2, typename ... Args>
    tuple_impl(const mpl::sequence<IIndex1,IIndex2,IIndexs...>&, Arg1&& i_arg1, Arg2&& i_arg2, Args&& ... i_args);
    template<typename Arg1, typename Arg2, typename ... Args>
    tuple_impl(Arg1&& i_arg1, Arg2&& i_arg2, Args&& ...vals);
    tuple_impl(const tuple_impl& other);
    tuple_impl(tuple_impl&& other);
    template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename TType1, typename TType2, typename ... TTypes>
    tuple_impl(const tuple_impl<mpl::sequence<IIndex1,IIndex2,IIndexs...>,TType1,TType2,TTypes...>& other);
    template<size_t IIndex1, size_t IIndex2, size_t ... IIndexs, typename TType1, typename TType2, typename ... TTypes>
    tuple_impl(tuple_impl<mpl::sequence<IIndex1,IIndex2,IIndexs...>,TType1,TType2,TTypes...>&& other);
    ~tuple_impl();
    tuple_impl& operator=(const tuple_impl& other);
    tuple_impl& operator=(tuple_impl&& other);
    template<size_t IIndex>
    typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::cref_type get() const;
    template<size_t IIndex>
    typename embedded_type<typename mpl::nth_type_of<IIndex,Type1,Type2,Types...>::type>::ref_type get();
    template<size_t ... IIndexs, typename ... Args>
    void set(const mpl::sequence<IIndexs...>&, Args&& ... i_args);
    template<size_t IIndex, typename Arg>
    bool set(Arg&& i_val);
    static constexpr size_t size();

private:
    typedef arena<mpl::get_total_size<Type1,Type2,Types...>::value,mpl::get_total_alignment<Type1,Type2,Types...>::value> data_type;

    template<typename TType, typename Arg>
    inline bool construct(void* i_address, Arg&& i_val)
    {
        return embedded_type<TType>::construct(i_address,i_val);
    }
    template<typename TType, typename Arg>
    inline bool assign(void* i_address, Arg&& i_val)
    {
        return embedded_type<TType>::assign(i_address,i_val);
    }
    template<typename TType>
    inline bool destruct(void* i_address)
    {
        return embedded_type<TType>::destroy(i_address);
    }
    template<typename TType>
    inline typename embedded_type<TType>::ref_type get(const void* i_address)
    {
        return reinterpret_cast<embedded_type<TType> *>(const_cast<void*>(i_address))->get();
    }
    template<typename TType>
    inline typename embedded_type<TType>::cref_type get(const void* i_address) const
    {
        return reinterpret_cast<embedded_type<TType> *>(const_cast<void*>(i_address))->get();
    }
    template<typename TType>
    inline typename embedded_type<TType>::pointer_type get_ptr(const void* i_address)
    {
        return reinterpret_cast<embedded_type<TType> *>(const_cast<void*>(i_address))->get_ptr();
    }
    template<typename TType>
    inline typename embedded_type<TType>::pointer_type get_ptr(const void* i_address) const
    {
        return reinterpret_cast<embedded_type<TType> *>(const_cast<void*>(i_address))->get_ptr();
    }
    template<typename TType>
    inline typename embedded_type<TType>::rref_type extract(void* i_address)
    {
        return reinterpret_cast<embedded_type<TType> *>(i_address)->extract();
    }

    data_type m_storage;
    typedef typename mpl::acc_sizeof<Type1,Type2,Types...>::type data_offset;
};

}

template<typename ... Types>
using tuple = detail::tuple_impl<typename mpl::make_sequence<0,mpl::get_num_types<Types...>::value>::type,Types...>;

template<typename ... Types>
tuple<Types&&...> make_tuple(Types&& ... vals);
template<typename ... TypesA, typename ... TypesB>
inline tuple<TypesA...,TypesB...> merge(const tuple<TypesA...>& i_lhs, const tuple<TypesB...>& i_rhs);
template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, typename ... TypesA, typename ... TypesB>
inline tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>& i_destSeq, const tuple<TypesA...>& i_lhs, const tuple<TypesB...>& i_rhs);
template<typename ... Types, typename ... Args>
inline tuple<Types...,Args...> merge_args(const tuple<Types...>& i_lhs, Args&& ... i_args);
template<typename ... FinalTypes, size_t ... FromIndexs, size_t ... ToIndexs, typename ... Types, typename ... Args>
inline tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>& i_destSeq, const tuple<Types...>& i_lhs, Args&& ... i_args);

}

namespace std
{

template<typename ... Types>
struct add_const<ddk::tuple<Types...>>
{
    typedef ddk::tuple<typename add_const<Types>::type ...> type;
};

}

#include "ddk_tuple.inl"
