#pragma once

#include <type_traits>
#include <cstring>
#include "ewas_void.h"
#include <functional>

#define ASSERT_CONTAINS_SYMBOL(_CLASS,_SYMBOL,_MSG) \
struct contains_symbol_##_CLASS##_##_SYMBOL \
{ \
private: \
	template<typename T> \
	static std::true_type ewas_contains_symbol(const T*, const typename T::_SYMBOL* = NULL); \
	static std::false_type ewas_contains_symbol(...); \
	typedef decltype(ewas_contains_symbol(reinterpret_cast<_CLASS*>(NULL))) res_type; \
public: \
	static const bool value = res_type::value; \
}; \
static_assert(contains_symbol_##_CLASS##_##_SYMBOL::value, _MSG);

namespace ewas
{
namespace mpl
{

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
struct size_of_qualified_type<T*>
{
	static const size_t value = sizeof(decltype(reinterpret_cast<T*>(NULL)));
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

template<size_t ... ranks>
struct sequence
{
    static const size_t size = sizeof...(ranks);
};

template<size_t, size_t,size_t...>
struct _create_range_rank;

template<size_t endRank, size_t currRank, size_t ... ranks>
struct _create_range_rank
{
    typedef typename _create_range_rank<endRank,currRank+1,ranks...,currRank>::type type;
};

template<size_t endRank, size_t ... ranks>
struct _create_range_rank<endRank,endRank,ranks...>
{
    typedef sequence<ranks...> type;
};

template<size_t initRank, size_t endRank>
struct create_range_rank
{
    typedef typename _create_range_rank<endRank,initRank>::type type;
};

template<typename ...>
struct get_total_size;

template<typename T, typename ... Types>
struct get_total_size<T,Types...>
{
    static const int value = sizeof(T) + get_total_size<Types...>::value;
};
template<>
struct get_total_size<>
{
    static const int value = 0;
};

template<typename ...>
struct get_num_types;

template<typename T, typename ... Types>
struct get_num_types<T,Types...>
{
    static const int value = 1 + get_num_types<Types...>::value;
};
template<>
struct get_num_types<>
{
    static const int value = 0;
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

template<int Pos>
struct nth_type_of<Pos>
{
    typedef detail::void_t type;
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

template<typename Type>
struct construct_type
{
	template<typename Arg>
	static Type with(Arg&& i_arg)
	{
		return Type{i_arg};
	}
};

template<typename>
struct aqcuire_callable_return_type;

template<typename Return, typename ... Args>
struct aqcuire_callable_return_type<std::function<Return(Args...)>>
{
	typedef Return return_type;
	typedef std::tuple<Args...> args_type;
	typedef std::function<Return(Args...)> func_type;
};
template<typename Return, typename T, typename ... Args>
struct aqcuire_callable_return_type<Return(T::*)(Args...) const>
{
	typedef Return return_type;
	typedef std::tuple<Args...> args_type;
	typedef std::function<Return(Args...)> func_type;
};
template<typename Functor>
struct aqcuire_callable_return_type
{
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::return_type return_type;
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::args_type args_type;
	typedef typename aqcuire_callable_return_type<decltype(&Functor::operator())>::func_type func_type;
};

}
}
