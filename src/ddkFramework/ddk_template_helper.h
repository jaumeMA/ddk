//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_void.h"
#include <type_traits>
#include <cstring>
#include <functional>
#include <memory>

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
inline constexpr bool evaluate_and()
{
    return (Conds && ...);
}
template<bool ... Conds>
inline constexpr bool evaluate_or()
{
    return (Conds || ...);
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

template<typename T>
struct class_holder
{
    typedef T type;

    constexpr class_holder() = default;
};

template<template<typename ...>typename T>
struct template_class_holder
{
public:
    template<typename ... TT>
    using type = T<TT...>;
};

template<typename T>
class_holder<T> resolve_holder();

template<template<typename...> typename T>
template_class_holder<T> resolve_holder();

template<typename T>
struct is_templated_class_holder : std::false_type
{
};
template<typename T>
struct is_templated_class_holder<class_holder<T>> : std::false_type
{
};
template<template<typename...> typename T>
struct is_templated_class_holder<template_class_holder<T>> : std::true_type
{
};

template<typename T>
inline constexpr bool is_templated_class_holder_v = is_templated_class_holder<T>::value;

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

template<typename>
struct add_const_impl;
    
template<typename T>
struct add_const_impl<T&>
{
    typedef const T& type;
};
template<typename T>
struct add_const_impl
{
    typedef const T type;
};

template<typename T>
using add_const = typename add_const_impl<T>::type;

template<typename T>
inline constexpr bool is_const = std::is_const<typename std::remove_reference<T>::type>::value;

template<typename T>
using remove_qualifiers = typename std::remove_const<typename std::remove_reference<T>::type>::type;

template<typename>
struct add_contravariant_constness_impl;
    
template<typename T>
struct add_contravariant_constness_impl
{
    typedef typename std::add_const<T>::type type;
};
template<template<typename> typename T, typename TT>
struct add_contravariant_constness_impl<T<TT>>
{
    typedef T<typename std::add_const<TT>::type> type;
};

template<typename T>
using add_contravariant_constness = typename add_contravariant_constness_impl<T>::type;

template<typename,typename>
struct _are_types_contravariant;

template<typename T>
struct _are_types_contravariant<T,T> : std::true_type
{
};
template<template<typename> typename T,typename ... TT,typename ... TTT>
struct _are_types_contravariant<T<TT...>,T<TTT...>> : std::true_type
{
};
template<typename T,typename TT>
struct _are_types_contravariant : std::false_type
{
};

template<typename T,typename TT>
inline constexpr bool are_types_contravariant = _are_types_contravariant<T,TT>::value;

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
template<size_t ... ranks>
constexpr size_t nth_rank_of(size_t pos)
{
    const size_t s_ranks[] = { ranks ...};

    return s_ranks[pos];
};

template<size_t ... ranks>
constexpr size_t get_num_ranks()
{
	return sizeof...(ranks);
};

template<typename T, typename ... TT>
constexpr inline auto get_min(T&& i_value, TT&& ... i_values)
{
    typedef typename std::common_type<T,TT...>::type res_type;

    res_type res = i_value;
    const res_type tail[] = { i_values... };

    for (auto&& value : tail)
    {
        res = (res > value) ? value : res;
    }

    return res;
};

template<size_t ... ranks>
inline constexpr size_t num_ranks = sizeof...(ranks);

template<size_t ... ranks>
constexpr size_t get_sum_ranks()
{
    return (ranks + ...);
}

template<size_t ... ranks>
inline constexpr size_t sum_ranks = (ranks + ...);

template<size_t ... ranks>
constexpr size_t get_prod_ranks()
{
    return (ranks * ...);
}

template<size_t ... ranks>
inline constexpr size_t prod_ranks = (ranks * ...);

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

template<size_t,size_t>
struct make_sequence;

template<typename...>
struct merge_sequence;

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
    inline static constexpr size_t size = sizeof...(ranks);
    template<size_t Index>
    inline static constexpr size_t nth_rank = nth_rank_of<ranks...>(Index);
    inline static constexpr size_t prod = ddk::mpl::prod_ranks<ranks...>;
    inline static constexpr size_t sum = ddk::mpl::sum_ranks<ranks...>;
    inline static constexpr size_t min = get_cond_rank<min_rank,ranks...>::value;
    inline static constexpr size_t max = get_cond_rank<max_rank,ranks...>::value;

    constexpr sequence() = default;

    template<typename>
    struct at;

    template<size_t ... Indexs>
    struct at<sequence<Indexs...>>
    {
        typedef sequence<nth_rank_of<ranks...>(Indexs) ...> type;
    };
    static constexpr bool present(size_t i_pos)
    {
        return ((i_pos == ranks) || ...);
    }
    static constexpr size_t find(size_t i_pos)
    {
        const size_t indexs[] = {ranks...};

        for(size_t index=0;index<num_ranks<ranks...>;++index)
        {
            if(indexs[index] == i_pos)
            {
                return index;
            }
        }

        return static_cast<size_t>(-1);
    }

    template<size_t Index>
    using drop = typename merge_sequence<typename at<typename make_sequence<0,(Index > 0) ? Index-1 : 0>::type>::type,typename at<typename make_sequence<Index+1,size>::type>::type>::type;
};

template<size_t ... ranksA, size_t ... ranksB>
struct merge_sequence<sequence<ranksA...>,sequence<ranksB...>>
{
    typedef sequence<ranksA...,ranksB...> type;
};

template<typename Sequence, typename ... Sequences>
struct merge_sequence<Sequence,Sequences...>
{
    typedef typename merge_sequence<Sequence,typename merge_sequence<Sequences...>::type>::type type;
};

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
        typedef sequence<sequence<Indexs...>::find(IIndexs) ...> type;
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

template<size_t Index, typename T>
using index_to_type = T;

template<typename T, size_t Index>
inline constexpr size_t type_to_index = Index;

template<size_t Index,size_t IIndex>
inline constexpr size_t index_to_index = IIndex;

template<typename T,typename TT>
using type_to_type = TT;

template<typename ... Types>
constexpr size_t get_num_types()
{
    return sizeof...(Types);
}

template<typename ... Types>
inline constexpr size_t num_types = sizeof...(Types);

template<template<typename> class predicate, typename ... Types>
constexpr size_t get_num_of_types_of()
{
    return (static_cast<size_t>(predicate<Types>::value) + ...);
};

template<template<typename> class predicate, typename ... Types>
inline constexpr size_t num_types_of = get_num_of_types_of<predicate,Types...>();

template<typename,typename>
struct is_same_type;
template<typename A,typename B>
struct is_same_type : std::false_type
{};
template<typename A>
struct is_same_type<A,A> : std::true_type
{};

template<typename T>
struct is_type
{
    template<typename TT>
    using type = is_same_type<T,TT>;
};

template<typename,typename>
struct is_not_same_type;
template<typename A,typename B>
struct is_not_same_type : std::true_type
{};
template<typename A>
struct is_not_same_type<A,A> : std::false_type
{};

template<typename T>
struct is_not_type
{
    template<typename TT>
    using type = is_not_same_type<T,TT>;
    template<typename TT>
    constexpr static bool value = is_not_same_type<T,TT>::value;
};

template<typename T>
inline constexpr bool is_void = is_type<T>::template type<void>::value;
template<typename T>
inline constexpr bool is_void_t = is_type<T>::template type<detail::void_t>::value;

template<typename T, typename ... Types>
constexpr bool are_same_type()
{
    return (std::is_same<T,Types>::value && ...);
}

template<template<typename...>typename Predicate, typename ... T>
struct not_predicate
{

template<typename ... TT>
using type = typename static_if<Predicate<T...,TT...>::value,std::false_type,std::true_type>::type;;

};

template<template<typename...> typename Predicate, typename ... T>
struct predicate
{

template<typename ... TT>
using type = typename static_if<Predicate<T...,TT...>::value,std::true_type,std::false_type>::type;;

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

template<size_t Pos, typename ... Types>
using nth_type_of_t = typename nth_type_of<Pos,Types...>::type;

template<typename...>
struct _reduce_to_common_type;

template<typename T, typename ... TT>
struct _reduce_to_common_type<T,TT...>
{
    typedef typename std::enable_if<are_same_type<T,TT...>(),T>::type type;
};

template<typename ... Types>
using reduce_to_common_type = typename _reduce_to_common_type<Types...>::type;

template<template<typename,typename...> typename Predicate, typename Type, typename ... Types>
inline constexpr size_t nth_pos_of_predicate()
{
    const bool predicateRes[num_types<Types...>] = { Predicate<Types,Type>::value ... };
    size_t pos = 0;

    while (predicateRes[pos] == false)
    {
        if (++pos == num_types<Types...>)
        {
            break;
        }
    }

    return pos;
}
template<template<typename,typename> typename Predicate, typename Type, typename ... Types>
inline constexpr bool holds_type_for_any_type()
{
    return (Predicate<Types,Type>::value && ...);
}

template<template<typename,typename> typename Predicate,typename Type,typename ... Types>
inline constexpr bool holds_type_for_by_type()
{
    return (Predicate<Type,Types>::value && ...);
}

template<template<typename> typename Predicate, typename ... Types>
inline constexpr bool holds_any_type()
{
    return (Predicate<Types>::value && ...);
}

template<template<typename,typename> typename Predicate, typename Type, typename ... Types>
inline constexpr bool holds_type_for_some_type()
{
    return (Predicate<Types,Type>::value || ...);
}

template<template<typename,typename> typename Predicate,typename Type,typename ... Types>
inline constexpr bool holds_type_by_some_type()
{
    return (Predicate<Type,Types>::value || ...);
}

template<template<typename> typename Predicate, typename ... Types>
inline constexpr bool holds_some_type()
{
    return (Predicate<Types>::value || ...);
}

template<typename Type, typename ... Types>
inline constexpr size_t pos_of_type = nth_pos_of_predicate<is_same_type,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_among_types = holds_type_for_some_type<is_same_type,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_not_among_types = (holds_type_for_some_type<is_same_type,Type,Types...>() == false);

template<typename Type,typename ... Types>
inline constexpr bool is_among_convertible_types = holds_type_for_some_type<std::is_convertible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_not_among_convertible_types = (holds_type_for_some_type<std::is_convertible,Type,Types...>() == false);

template<typename Type,typename ... Types>
inline constexpr bool is_among_constructible_types = holds_type_for_some_type<std::is_constructible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_not_among_constructible_types = (holds_type_for_some_type<std::is_constructible,Type,Types...>() == false);

template<typename Type,typename ... Types>
inline constexpr bool is_some_convertible_type = holds_type_by_some_type<std::is_convertible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_not_any_convertible_type = (holds_type_by_some_type<std::is_convertible,Type,Types...>() == false);

template<typename Type,typename ... Types>
inline constexpr bool is_some_constructible_type = holds_type_by_some_type<std::is_constructible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr bool is_not_any_constructible_type = (holds_type_by_some_type<std::is_constructible,Type,Types...>() == false);


template<typename Type,typename ... Types>
inline constexpr size_t first_same_type = nth_pos_of_predicate<is_same_type,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr size_t first_constructible_type = nth_pos_of_predicate<std::is_constructible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr size_t first_convertible_type = nth_pos_of_predicate<std::is_convertible,Type,Types...>();

template<typename Type,typename ... Types>
inline constexpr size_t type_match_pos = (first_same_type<remove_qualifiers<Type>,remove_qualifiers<Types>...> != get_num_types<Types...>()) ? first_same_type<remove_qualifiers<Type>,remove_qualifiers<Types>...> : first_constructible_type<Type,Types...>;

template<typename Type>
struct construct_type
{
	template<typename Arg>
	static Type with(Arg&& i_arg)
	{
		return Type{i_arg};
	}
};

template<typename...>
struct merge_type_packs;

template<typename ... Types>
struct type_pack
{
    constexpr type_pack() = default;
	template<typename ... TTypes>
	struct add
	{
        static_assert(type_pack<Types...>::template contains<TTypes...>()==false, "Trying to add already added type.");
        
        typedef type_pack<Types...,TTypes...> type;
	};
	template<typename TType>
	struct add_unique
	{
        typedef typename which_type<is_among_types<TType,Types...>,type_pack<Types...>,type_pack<Types...,TType>>::type type;
	};
	template<typename ... TTypes>
	struct drop
	{
		typedef typename merge_type_packs<typename static_if<is_among_types<Types,TTypes...>,type_pack<>,type_pack<Types>>::type ...>::type type;
	};
    template<template<typename> typename Predicate>
    struct drop_if
    {
        typedef typename merge_type_packs<typename static_if<Predicate<Types>::value,type_pack<>,type_pack<Types>>::type ...>::type type;
    };
    template<typename Type, template<typename,typename> typename Predicate = is_same_type>
    static constexpr size_t pos_in_type_pack()
    {
        return nth_pos_of_predicate<Predicate,Type,Types...>();
    }
    template<typename ... TTypes>
	static constexpr bool contains(const type_pack<TTypes...>&)
	{
		return (num_types<TTypes...> == 0) || (is_among_types<TTypes, Types...> && ...);
	}
    template<typename ... TTypes>
    static constexpr bool contains()
    {
        return (num_types<TTypes...> == 0) || (is_among_types<TTypes,Types...> && ...);
    }
    template<typename ... TTypes>
    static constexpr bool projects(const type_pack<TTypes...>&)
    {
        return (num_types<TTypes...> == 0) || (is_among_constructible_types<TTypes,Types...> && ...);
    }
    template<typename ... TTypes>
    static constexpr bool projects()
    {
        return (num_types<TTypes...> == 0) || (is_among_constructible_types<TTypes,Types...> && ...);
    }
    template<size_t Index>
    struct at
    {
        typedef typename nth_type_of<Index,Types...>::type type;
    };

    template<typename>
    struct subset;
    template<size_t ... Indexs>
    struct subset<sequence<Indexs...>>
    {
        typedef type_pack<typename nth_type_of<Indexs,Types...>::type ...> type;
    };

    template<int Index>
    using nth_type = typename nth_type_of<Index,Types...>::type;

    static constexpr size_t size()
    {
        return num_types<Types...>;
    }
    static constexpr bool empty()
    {
        return num_types<Types...> == 0;
    }
};
typedef type_pack<> empty_type_pack;

template<typename...>
struct merge_pair_type_packs;

template<typename ... Types>
struct merge_pair_type_packs<type_pack<Types...>,type_pack<>>
{
    typedef type_pack<Types...> type;
};

template<typename ... Types,typename TType,typename ... TTypes>
struct merge_pair_type_packs<type_pack<Types...>,type_pack<TType,TTypes...>>
{
    typedef typename merge_pair_type_packs<typename type_pack<Types...>::template add_unique<TType>::type,type_pack<TTypes...>>::type type;
};

template<>
struct merge_type_packs<>
{
    typedef empty_type_pack type;
};
template<typename ... Types>
struct merge_type_packs<type_pack<Types...>>
{
    typedef type_pack<Types...> type;
};
template<typename ... Types, typename ... TTypes,typename ... TTTypes>
struct merge_type_packs<type_pack<Types...>,type_pack<TTypes...>,TTTypes...>
{
    typedef typename merge_type_packs<typename merge_pair_type_packs<type_pack<Types...>,type_pack<TTypes...>>::type,TTTypes...>::type type;
};

template<typename ... Types>
using type_pack_union = typename merge_type_packs<Types...>::type;

template<typename...>
struct intersect_type_packs;

template<typename ... Types, typename ... TTypes, typename ... TTTypes>
struct intersect_type_packs<type_pack<Types...>,type_pack<TTypes...>,TTTypes...>
{
    typedef typename intersect_type_packs<typename merge_type_packs<typename static_if<is_among_types<Types,TTypes...>,type_pack<Types>,type_pack<>>::type...>::type,TTTypes...>::type type;
};

template<typename ... Types>
struct intersect_type_packs<type_pack<Types...>>
{
    typedef type_pack<Types...> type;
};

template<typename ... Types>
using type_pack_intersection = typename intersect_type_packs<Types...>::type;

template<typename>
struct is_type_pack;

template<typename T>
struct is_type_pack
{
    static const bool value = false;
};

template<typename ... Types>
struct is_type_pack<type_pack<Types...>>
{
    static const bool value = true;
    static const size_t num_types = mpl::num_types<Types...>;
    template<typename Type>
    inline constexpr bool is_among_types()
    {
        return mpl::is_among_types<Type,Types...>;
    }
};

template<typename Type, typename ... Types>
inline constexpr bool is_among_type_pack = (is_type_pack<Types...>::value && is_type_pack<Types...>::template is_among_types<Type>()) || is_among_types<Type,Types...>;

template<typename,typename>
struct convertible_type_equivalence_type_pack;

template<typename ReferenceType, typename ... Types>
struct convertible_type_equivalence_type_pack<ReferenceType,type_pack<Types...>>
{
    typedef type_pack<Types...> type;
};

template<int ...ranks>
inline bool constexpr check_monotonic_range()
{
    return (ranks < ...);
}

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
inline constexpr size_t total_alignment = mpl::sequence<std::alignment_of<T>::value ...>::max;

template<typename ... T>
constexpr size_t total_size()
{
    void* arena = nullptr;
    const size_t k_maxSpace = static_cast<size_t>(-1);
    size_t spaceLeft = k_maxSpace;
    const size_t typeSizes[] = { mpl::size_of_qualified_type<T>::value ... };
    const size_t typeAlignments[] = { alignof(T) ... };

    for(size_t typeIndex = 0; typeIndex < num_types<T...>; ++typeIndex)
    {
        std::align(typeAlignments[typeIndex],typeSizes[typeIndex],arena,spaceLeft);
        arena = reinterpret_cast<char*>(arena) + typeSizes[typeIndex];
        spaceLeft -= typeSizes[typeIndex];
    }

    return k_maxSpace - spaceLeft;
}

template<typename>
struct _acc_sizeof;

template<size_t ... Indexs>
struct _acc_sizeof<sequence<Indexs...>>
{
private:
    static const size_t s_numIndexs = get_num_ranks<Indexs...>();

public:
    static const size_t value[s_numIndexs];

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
const size_t _acc_sizeof<sequence<Indexs...>>::value[s_numIndexs] = { Indexs ... };

template<typename ... Types>
struct acc_sizeof
{
    typedef _acc_sizeof<typename acc_sequence<size_of_qualified_type<Types>::value ...>::type> type;
};

constexpr bool is_power_of_two(size_t i_value)
{
    return (i_value & (i_value - 1)) == 0;
}
constexpr unsigned short next_power_of_two(unsigned short i_value)
{
    i_value--;
    i_value |= i_value >> 1;
    i_value |= i_value >> 2;
    i_value |= i_value >> 4;
    i_value |= i_value >> 8;
    i_value++;

    return i_value;
}
constexpr unsigned int next_power_of_two(unsigned int i_value)
{
    i_value--;
    i_value |= i_value >> 1;
    i_value |= i_value >> 2;
    i_value |= i_value >> 4;
    i_value |= i_value >> 8;
    i_value |= i_value >> 16;
    i_value++;

    return i_value;
}
constexpr unsigned int next_power_of_two(unsigned long int i_value)
{
    i_value--;
    i_value |= i_value >> 1;
    i_value |= i_value >> 2;
    i_value |= i_value >> 4;
    i_value |= i_value >> 8;
    i_value |= i_value >> 16;
    i_value++;

    return i_value;
}
constexpr unsigned long long int next_power_of_two(unsigned long long int i_value)
{
    i_value--;
    i_value |= i_value >> 1;
    i_value |= i_value >> 2;
    i_value |= i_value >> 4;
    i_value |= i_value >> 8;
    i_value |= i_value >> 16;
    i_value |= i_value >> 32;
    i_value++;

    return i_value;
}
constexpr unsigned char get_power_of_two(unsigned long long i_value)
{
    unsigned int powerOfTwo = 0;

    while (i_value >>= 1)
    {
        powerOfTwo++;
    }

    return powerOfTwo;
}

}
}
