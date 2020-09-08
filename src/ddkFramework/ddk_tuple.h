#pragma once

#include "ddk_embedded_type.h"
#include "ddk_template_helper.h"
#include "ddk_arena.h"

namespace ddk
{

template<typename ...>
class tuple;

template<>
class tuple<>
{
public:
    tuple() = default;
};

template<typename Type, typename ... Types>
class tuple<Type,Types...>
{
    template<typename...>
    friend class tuple;

public:
    static const size_t size = mpl::get_num_types<Type,Types...>::value;

    tuple<Type,Types...>() = default;
    template<size_t Index, size_t ... Indexs, typename Arg, typename ... Args>
    tuple(const mpl::sequence<Index,Indexs...>&, Arg&& i_arg, Args&& ... i_args);
    template<typename Arg, typename ... Args>
    tuple(Arg&& i_val, Args&& ...vals);
    tuple(const tuple<Type,Types...>& other);
    tuple(tuple<Type,Types...>&& other);
    template<typename Arg, typename ... Args>
    tuple(const tuple<Arg,Args...>& other);
    template<typename Arg, typename ... Args>
    tuple(tuple<Arg,Args...>&& other);
    ~tuple();
    tuple<Type,Types...>& operator=(const tuple<Type,Types...>& other);
    tuple<Type,Types...>& operator=(tuple<Type,Types...>&& other);
    template<int Index>
    typename embedded_type<typename mpl::nth_type_of<Index,Type,Types...>::type>::cref_type get() const;
    template<int Index>
    typename embedded_type<typename mpl::nth_type_of<Index,Type,Types...>::type>::ref_type get();
    template<int ... Indexs>
    void set(typename mpl::nth_type_of<Indexs,Type,Types...>::type ... args);
    template<int Index>
    bool set(typename mpl::nth_type_of<Index,Type,Types...>::type val);

private:
    typedef arena<mpl::get_total_size<Type,Types...>::value,mpl::get_total_alignment<Type,Types...>::value> data_type;

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
    typedef typename mpl::acc_sizeof<Type,Types...>::type data_offset;
};

template<typename ... Types>
tuple<Types&&...> make_tuple(Types&& ... vals)
{
    return tuple<Types&&...>(std::forward<Types>(vals)...);
}

template<size_t ... IndexsA, typename ... TypesA, size_t ... IndexsB, typename ... TypesB, size_t ... FromIndexs, size_t ... ToIndexs>
inline tuple<typename mpl::nth_type_of<ToIndexs,TypesA...,TypesB...>::type ...> merge(const mpl::sequence<IndexsA...>&, const tuple<TypesA...>& i_lhs, const mpl::sequence<IndexsB...>&, const tuple<TypesB...>& i_rhs, const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>&)
{
    return tuple<typename mpl::nth_type_of<ToIndexs,TypesA...,TypesB...>::type ...>(i_srcSeq,i_lhs.template get<IndexsA>() ..., i_rhs.template get<IndexsB>() ...);
}
template<typename ... TypesA, typename ... TypesB, size_t ... FromIndexs, size_t ... ToIndexs>
inline tuple<typename mpl::nth_type_of<ToIndexs,TypesA...,TypesB...>::type ...> merge(const tuple<TypesA...>& i_lhs, const tuple<TypesB...>& i_rhs, const mpl::sequence<FromIndexs...>& i_srcSeq, const mpl::sequence<ToIndexs...>& i_destSeq)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>::value>::type sequenceA;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesB...>::value>::type sequenceB;

    return merge(sequenceA{},i_lhs,sequenceB{},i_rhs,i_srcSeq,i_destSeq);
}

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
