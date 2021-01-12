#pragma once

#include "ddk_void.h"
#include <type_traits>
#include <cstring>
#include <functional>

#define CONTAINS_SYMBOL(_SYMBOL) \
template<typename T> \
struct contains_symbol_##_SYMBOL \
{ \
private: \
	template<typename TT> \
	static std::true_type _contains_symbol(TT&&, const typename TT::_SYMBOL* = nullptr); \
	static std::false_type _contains_symbol(...); \
	typedef decltype(_contains_symbol(std::declval<T>())) res_type; \
public: \
	static const bool value = res_type::value; \
};

#define ASSERT_CONTAINS_SYMBOL(_CLASS,_SYMBOL,_MSG) \
CONTAINS_SYMBOL(_SYMBOL) \
static_assert(contains_symbol_##_SYMBOL<_CLASS>::value, _MSG);

namespace ddk
{
namespace mpl
{

template<bool ... Conds>
constexpr bool evaluate()
{
    return (Conds && ...);
}

template<bool,typename,typename>
struct static_if;

template<typename T,typename TT>
struct static_if<true,T,TT>
{
    typedef T type;
};

template<typename T,typename TT>
struct static_if<false,T,TT>
{
    typedef TT type;
};

template<bool,typename,typename>
struct which_type;

template<typename T1, typename T2>
struct which_type<true,T1,T2>
{
	typedef T1 type;
};
template<typename T1, typename T2>
struct which_type<false,T1,T2>
{
	typedef T2 type;
};

template<typename>
struct is_copy_constructible;

template<typename>
struct is_copy_constructible
{
    static const bool value = true;
};

template<typename T>
struct size_of_qualified_type;

template<typename T>
struct size_of_qualified_type
{
	static const size_t value = sizeof(T);
};

template<typename T>
struct size_of_qualified_type<T&>
{
	static const size_t value = sizeof(decltype(reinterpret_cast<T*>(NULL)));
};

template<typename T>
struct size_of_qualified_type<T&&>
{
	static const size_t value = sizeof(decltype(reinterpret_cast<T*>(NULL)));
};

template<typename T>
struct size_of_qualified_type<T*>
{
	static const size_t value = sizeof(decltype(reinterpret_cast<T*>(NULL)));
};

template<typename T>
struct get_pointer
{
private:
	template<typename TT>
	static TT _get(TT&&, typename TT::pointer* = nullptr);
	template<typename TT>
	static TT* _get(TT*);
	template<typename TT>
	static TT* _get(const TT&);
	template<typename TT>
	static typename TT::const_type _get_const(TT&&,typename TT::const_pointer* = nullptr);
	template<typename TT>
	static const TT* _get_const(TT*);
	template<typename TT>
	static const TT* _get_const(const TT&);
	template<typename TT>
	static typename TT::pointer _get_pointer(TT&&,typename TT::pointer* = nullptr);
	template<typename TT>
	static TT* _get_pointer(TT*);
	template<typename TT>
	static TT* _get_pointer(const TT&);
	template<typename TT>
	static typename TT::const_pointer _get_const_pointer(TT&&,typename TT::const_pointer* = nullptr);
	template<typename TT>
	static const TT* _get_const_pointer(TT*);
	template<typename TT>
	static const TT* _get_const_pointer(const TT&);
	template<typename TT>
	static typename TT::reference _get_reference(TT&&,typename TT::reference* = nullptr);
	template<typename TT>
	static TT& _get_reference(TT*);
	template<typename TT>
	static TT& _get_reference(const TT&);
	template<typename TT>
	static typename TT::reference _get_const_reference(TT&&,typename TT::const_reference* = nullptr);
	template<typename TT>
	static const TT& _get_const_reference(TT*);
	template<typename TT>
	static const TT& _get_const_reference(const TT&);

public:
	typedef decltype(_get(std::declval<T>())) type;
	typedef decltype(_get_const(std::declval<T>())) const_type;
	typedef decltype(_get_pointer(std::declval<T>())) pointer;
	typedef decltype(_get_const_pointer(std::declval<T>())) const_pointer;
	typedef decltype(_get_reference(std::declval<T>())) reference;
	typedef decltype(_get_const_reference(std::declval<T>())) const_reference;
};

template<typename...>
struct max_type;

template<typename Type1, typename Type2, typename ... Types>
struct max_type<Type1,Type2,Types...>
{
	typedef typename max_type<typename which_type<(size_of_qualified_type<Type1>::value > size_of_qualified_type<Type2>::value),Type1,Type2>::type,Types...>::type type;
};

template<typename Type>
struct max_type<Type>
{
	typedef Type type;
};

template<size_t rank1, size_t rank2>
struct max_rank
{
    static const bool value  = (rank1 > rank2);
};

template<size_t rank1, size_t rank2>
struct min_rank
{
    static const bool value  = (rank1 < rank2);
};

//ranks
template<size_t pos, size_t ... ranks>
struct nth_rank_of;

template<size_t pos, size_t rank, size_t ... ranks>
struct nth_rank_of<pos,rank,ranks...>
{
    static const size_t value = nth_rank_of<pos-1,ranks...>::value;
};

template<size_t rank, size_t ... ranks>
struct nth_rank_of<0,rank,ranks...>
{
    static const size_t value = rank;
};

template<size_t ... ranks>
struct get_num_ranks
{
	static const size_t value = sizeof...(ranks);
};

template<template<size_t,size_t> class cond, size_t ... ranks>
struct get_cond_rank;

template<template<size_t,size_t> class cond, size_t rank>
struct get_cond_rank<cond,rank>
{
    static const size_t value = rank;
};

template<template<size_t,size_t> class cond, size_t rank1, size_t rank2, size_t ... ranks>
struct get_cond_rank<cond,rank1,rank2,ranks...>
{
    static const size_t value = (cond<rank1,rank2>::value) ? get_cond_rank<cond,rank1,ranks...>::value : get_cond_rank<cond,rank2,ranks...>::value;
};

template<size_t Index>
struct static_number{};

template<size_t ...>
struct sequence;

template<>
struct sequence<>
{
    template<typename>
    struct at
    {
        typedef sequence<> type;
    };
};

template<size_t ... ranks>
struct sequence
{
    static const size_t size = 1 + sizeof...(ranks);

    template<typename>
    struct at;

    template<size_t ... Indexs>
    struct at<sequence<Indexs...>>
    {
        typedef sequence<nth_rank_of<Indexs,ranks...>::value ...> type;
    };

    template<size_t Index, size_t Pos = 0>
    struct find;

    template<size_t Index>
    struct find<Index,size>
    {
        static const size_t index = -1;
    };

    template<size_t Index, size_t Pos>
    struct find
    {
    private:
        template<bool,typename>
        struct found;
        template<typename T>
        struct found<true,T>
        {
            static const size_t index = Pos;
        };
        template<typename T>
        struct found<false,T>
        {
            static const size_t index = find<Index,Pos+1>::index;
        };

    public:
        static const size_t index = found<Index==nth_rank_of<Pos,ranks...>::value,void>::index;
    };

    static const size_t min = get_cond_rank<min_rank,ranks...>::value;
    static const size_t max = get_cond_rank<max_rank,ranks...>::value;
};

template<typename,typename>
struct merge_sequence;

template<size_t ... ranksA, size_t ... ranksB>
struct merge_sequence<sequence<ranksA...>,sequence<ranksB...>>
{
    typedef sequence<ranksA...,ranksB...> type;
};

template<size_t ini, size_t end>
struct make_sequence;

template<size_t end>
struct make_sequence<end,end>
{
    typedef sequence<> type;
};

template<size_t ini, size_t end>
struct make_sequence
{
    typedef typename merge_sequence<sequence<ini>,typename make_sequence<ini+1,end>::type>::type type;
};

template<typename>
struct dual_sequence;

template<>
struct dual_sequence<sequence<>>
{

template<size_t ini, size_t end>
struct at
{
    typedef typename make_sequence<ini,end>::type type;
};

};

template<size_t rank, size_t ... ranks>
struct dual_sequence<mpl::sequence<rank,ranks...>>
{

template<size_t ini, size_t end>
struct at;

template<size_t ini>
struct at<ini,ini>
{
    typedef sequence<> type;
};

template<size_t ini, size_t end>
struct at
{
    typedef typename merge_sequence<typename make_sequence<ini,rank>::type,typename dual_sequence<mpl::sequence<ranks...>>::template at<rank+1,end>::type>::type type;
};

};

template<typename>
struct inverse_sequence;

template<size_t ... Indexs>
struct inverse_sequence<sequence<Indexs...>>
{
private:
    template<typename>
    struct resolve;
    template<size_t ... IIndexs>
    struct resolve<sequence<IIndexs...>>
    {
        typedef sequence<sequence<Indexs...>::template find<IIndexs>::index ...> type;
    };

public:
    typedef typename resolve<typename make_sequence<sequence<Indexs...>::min,sequence<Indexs...>::max+1>::type>::type type;
};

template<size_t ... ranks>
struct acc_sequence;

template<>
struct acc_sequence<>
{
    template<size_t carry, size_t ... otherRanks>
    struct _partial
    {
        typedef sequence<otherRanks...> type;
    };

	typedef sequence<> type;
};

template<size_t rank, size_t ... ranks>
struct acc_sequence<rank,ranks...>
{
    template<size_t carry, size_t ... otherRanks>
    struct _partial
    {
        typedef typename acc_sequence<ranks...>::template _partial<rank+carry,otherRanks...,rank+carry>::type type;
    };

typedef typename _partial<0>::type type;
};

template<size_t...>
struct prod_ranks;

template<>
struct prod_ranks<>
{
	static const size_t value = 1;
};

template<size_t rank, size_t ... ranks>
struct prod_ranks<rank,ranks...>
{
	static const size_t value = rank * prod_ranks<ranks...>::value;
};

template<typename ... Types>
struct get_num_types
{
    static const int value = sizeof...(Types);
};

template<template<typename> class,typename ...>
struct get_num_of_types_of;

template<template<typename> class predicate, typename Type, typename ... Types>
struct get_num_of_types_of<predicate,Type,Types...>
{
    static const int value = predicate<Type>::value + get_num_of_types_of<predicate,Types...>::value;
};
template<template<typename> class predicate>
struct get_num_of_types_of<predicate>
{
    static const int value = 0;
};

template<typename A, typename B>
struct is_same_type;

template<typename A, typename B>
struct is_same_type
{
    static const bool value = false;
};

template<typename A>
struct is_same_type<A,A>
{
    static const bool value = true;
};

template<typename ...>
struct homogeneous_types;

template<typename Type, typename ... Types>
struct homogeneous_types<Type,Types...>
{
    static const bool value = is_same_type<Type,typename homogeneous_types<Types...>::type>::value && homogeneous_types<Types...>::value;
    typedef Type type;
};

template<typename Type>
struct homogeneous_types<Type>
{
    static const bool value = true;
    typedef Type type;
};

template<int, typename ...>
struct nth_type_of;

template<int Pos, typename Type, typename ... Types>
struct nth_type_of<Pos,Type,Types...>
{
    typedef typename nth_type_of<Pos-1,Types...>::type type;
};

template<typename Type, typename ... Types>
struct nth_type_of<0,Type,Types...>
{
    typedef Type type;
};

template<template <class, class...> class predicate, bool cond, int _pos, typename ... Types>
struct _nth_pos_of_predicate;

template<template <class, class...> class predicate, int _pos, typename TType, typename Type, typename ... Types>
struct _nth_pos_of_predicate<predicate,false,_pos,TType,Type,Types...>
{
    static const size_t value = _nth_pos_of_predicate<predicate, predicate<Type,TType>::value, _pos + 1, TType, Types...>::value;
};

template<template <class, class...> class predicate, int _pos, typename Type, typename ... Types>
struct _nth_pos_of_predicate <predicate,true,_pos,Type,Types... >
{
    static const size_t value = _pos;
};

template<template <class, class...> class predicate, int _pos, typename Type>
struct _nth_pos_of_predicate<predicate,false,_pos,Type>
{
    static const size_t value = -1;
};

template<template <class,class...> class predicate, typename Type, typename ... Types>
struct nth_pos_of_predicate;

template<template <class, class...> class predicate, typename TType, typename Type, typename ... Types>
struct nth_pos_of_predicate<predicate,TType,Type,Types...>
{
    static const size_t value = _nth_pos_of_predicate<predicate, predicate<Type, TType>::value, 0, TType, Types...>::value;
};

template<template <class, class...> class predicate, typename Type>
struct nth_pos_of_predicate<predicate,Type>
{
    static const size_t value = -1;
};

template<typename Type, typename ... Types>
struct nth_pos_of_type
{
    static const size_t value = nth_pos_of_predicate<is_same_type,Type,Types...>::value;
};

template<typename Type, typename ... Types>
struct is_among_types
{
    static const bool value = (nth_pos_of_predicate<is_same_type,Type,Types...>::value != -1);
};

template<typename Type, typename ... Types>
struct is_among_convertible_types
{
    static const bool value = (nth_pos_of_predicate<std::is_convertible,Type,Types...>::value != -1);
};

template<typename Type,typename ... Types>
struct is_among_constructible_types
{
	static const bool value = (nth_pos_of_predicate<std::is_constructible,Type,Types...>::value != -1);
};

template<typename Type, typename ... Types>
struct get_first_constructible_type
{
private:
	static const size_t typePos = nth_pos_of_predicate<std::is_constructible,Type,Types...>::value;

public:
	//placeholder
	typedef typename nth_type_of<typePos,Types...>::type type;
	static const size_t value = typePos;
};

template<typename Type, typename ... Types>
struct get_first_convertible_type
{
private:
	static const size_t typePos = nth_pos_of_predicate<std::is_convertible,Type,Types...>::value;

public:
	//placeholder
	typedef typename nth_type_of<typePos,Types...>::type type;
	static const size_t value = typePos;
};

template<typename Type,typename ... Types>
struct get_type_match_pos
{
private:
	typedef typename std::remove_const<typename std::remove_reference<Type>::type>::type raw_type;

    static const size_t npos = static_cast<size_t>(-1);
	static const size_t typePosSame = nth_pos_of_predicate<std::is_same,raw_type,Types...>::value;
	static const size_t typePosCtr = nth_pos_of_predicate<std::is_constructible,Type,Types...>::value;
	static const size_t typePos = (typePosSame != npos) ? typePosSame : typePosCtr;

public:
	//placeholder
	typedef typename nth_type_of<typePos,Types...>::type type;
	static const size_t value = typePos;
};

template<typename Type>
struct construct_type
{
	template<typename Arg>
	static Type with(Arg&& i_arg)
	{
		return Type{i_arg};
	}
};

template<typename ... Types>
struct type_pack
{
	template<typename TType>
	struct add
	{
		typedef typename static_if<is_among_types<TType,Types...>::value,type_pack<Types...>,type_pack<Types...,TType>>::type type;
	};
};

template<int ...ranks>
struct check_monotonic_range;

template<int rank, int ...ranks>
struct check_monotonic_range<rank,ranks...>
{
    static const bool cond = rank < check_monotonic_range<ranks...>::_rank && check_monotonic_range<ranks...>::cond;
    static const int _rank = rank;
};

template<>
struct check_monotonic_range<>
{
    static const bool cond = true;
    static const size_t _rank = -1;
};

template<int rankA, int rankB>
struct is_same_rank
{
    static const bool value = rankA == rankB;
};

template<typename ...>
struct total_type;

template<typename T, typename ... TT>
struct total_type<T,TT...> : total_type<TT...>
{
	T _;
};

template<>
struct total_type<>
{
};

template<typename ... T>
struct get_total_alignment
{
    static const size_t value = mpl::sequence<std::alignment_of<T>::value ...>::max;
};

template<typename ... T>
struct get_total_size
{
	static const size_t value = sizeof(total_type<T...>);
};

template<typename>
struct _acc_sizeof;

template<size_t ... Indexs>
struct _acc_sizeof<sequence<Indexs...>>
{
    static const size_t value[get_num_ranks<Indexs...>::value];

    constexpr _acc_sizeof() = default;

    inline static size_t at(size_t i_index)
    {
        return (i_index == 0) ? 0 : value[i_index-1];
    }
	static constexpr size_t index(size_t i_ptrDiff, size_t i_initialIndex = 0)
    {
        return (i_ptrDiff == 0) ? 0 : (value[i_initialIndex] == i_ptrDiff) ? i_initialIndex + 1 : index(i_ptrDiff,i_initialIndex + 1);
    }
};

template<size_t ... Indexs>
const size_t _acc_sizeof<sequence<Indexs...>>::value[get_num_ranks<Indexs...>::value] = { Indexs ... };

template<typename ... Types>
struct acc_sizeof
{
    typedef _acc_sizeof<typename acc_sequence<size_of_qualified_type<Types>::value ...>::type> type;
};

template<template<typename> class, typename ...>
struct fullfils_predicate;

template<template<typename> class P>
struct fullfils_predicate<P>
{
	static const bool value = true;
};
template<template<typename> class P, typename T, typename ... TT>
struct fullfils_predicate<P,T,TT...>
{
	static const bool value = P<T>::value && fullfils_predicate<P,TT...>::value;
};

}
}
