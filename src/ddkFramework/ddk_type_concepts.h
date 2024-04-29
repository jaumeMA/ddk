//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_template_helper.h"
#include "ddk_scoped_enum.h"

#define IS_NUMBER_OF_ARGS_COND(_NUM,...) \
    (ddk::mpl::num_types<__VA_ARGS__> == _NUM)

#define IS_NUMBER_OF_ARGS(_NUM,...) \
    typename std::enable_if<IS_NUMBER_OF_ARGS_COND(_NUM,__VA_ARGS__)>::type

#define IS_NUMBER_OF_ARGS_GREATER_COND(_NUM,...) \
    (ddk::mpl::num_types<__VA_ARGS__> >= (_NUM+1))

#define IS_NUMBER_OF_ARGS_GREATER(_NUM,...) \
    typename std::enable_if<IS_NUMBER_OF_ARGS_GREATER_COND(_NUM,__VA_ARGS__)>::type

#define IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL_COND(_NUM,...) \
    (ddk::mpl::num_types<__VA_ARGS__> >= _NUM)

#define IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(_NUM,...) \
    typename std::enable_if<IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL_COND(_NUM,__VA_ARGS__)>::type

#define IS_NUMBER_OF_ARGS_LESSER_COND(_NUM,...) \
    ((ddk::mpl::num_types<__VA_ARGS__>+1) <= _NUM)

#define IS_NUMBER_OF_ARGS_LESSER(_NUM,...) \
    typename std::enable_if<IS_NUMBER_OF_ARGS_LESSER_COND(_NUM,__VA_ARGS__)>::type

#define IS_NUMBER_OF_ARGS_LESSER_OR_EQUAL_COND(_NUM,...) \
    (ddk::mpl::num_types<__VA_ARGS__> <= _NUM)

#define IS_NUMBER_OF_ARGS_LESSER_OR_EQUAL(_NUM,...) \
    typename std::enable_if<IS_NUMBER_OF_ARGS_LESSER_OR_EQUAL_COND(_NUM,__VA_ARGS__)>::type

#define IS_AMONG_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_among_types<_TYPE,__VA_ARGS__>

#define IS_AMONG_TYPES(_TYPE,...) \
	typename std::enable_if<IS_AMONG_TYPES_COND(_TYPE,__VA_ARGS__)>::type

#define IS_NOT_AMONG_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_not_among_types<_TYPE,__VA_ARGS__>

#define IS_NOT_AMONG_TYPES(_TYPE,...) \
	typename std::enable_if<IS_NOT_AMONG_TYPES_COND(_TYPE,__VA_ARGS__)>::type

#define IS_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>

#define IS_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<IS_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,__VA_ARGS__)>::type

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_not_among_constructible_types<_TYPE,__VA_ARGS__>

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<IS_NOT_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,__VA_ARGS__)>::type

#define IS_BASE_OF_COND(_BASE,_TYPE) \
	std::is_base_of<_BASE,_TYPE>::value

#define IS_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<IS_BASE_OF_COND(_BASE,_TYPE)>::type

#define IS_NOT_BASE_OF_COND(_BASE,_TYPE) \
	(std::is_base_of<_BASE,_TYPE>::value == false)

#define IS_NOT_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<IS_NOT_BASE_OF_COND(_BASE,_TYPE)>::type

#define IS_BINDABLE_BY_COND(_TYPE,_TTYPE) \
	ddk::concepts::is_bindable_by<_TYPE,_TTYPE>

#define IS_BINDABLE_BY(_TYPE,_TTYPE) \
	typename std::enable_if<IS_BINDABLE_BY_COND(_TYPE,_TTYPE)>::type

#define IS_SAME_CLASS_COND(_TYPE,__TYPE) \
	std::is_same<_TYPE,__TYPE>::value

#define IS_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<IS_SAME_CLASS_COND(_TYPE,__TYPE)>::type

#define IS_NOT_SAME_CLASS_COND(_TYPE,__TYPE) \
	(std::is_same<_TYPE,__TYPE>::value==false)

#define IS_NOT_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<IS_NOT_SAME_CLASS_COND(_TYPE,__TYPE)>::type

#define IS_CLASS_COND(_TYPE) \
	std::is_class<_TYPE>::value

#define IS_CLASS(_TYPE) \
	typename std::enable_if<IS_CLASS_COND(_TYPE)>::type

#define IS_NOT_CLASS_COND(_TYPE) \
	(std::is_class<_TYPE>::value == false)

#define IS_NOT_CLASS(_TYPE) \
	typename std::enable_if<IS_NOT_CLASS_COND(_TYPE)>::type

#define IS_DEFAULT_CONSTRUCTIBLE_COND(_TYPE) \
	std::is_default_constructible_v<_TYPE>

#define IS_DEFAULT_CONSTRUCTIBLE(_TYPE) \
	typename std::enable_if<IS_DEFAULT_CONSTRUCTIBLE_COND(_TYPE)>::type

#define IS_CONSTRUCTIBLE_COND(_TYPE,...) \
	std::is_constructible_v<_TYPE,__VA_ARGS__>

#define IS_CONSTRUCTIBLE(_TYPE,...) \
	typename std::enable_if<IS_CONSTRUCTIBLE_COND(_TYPE,__VA_ARGS__)>::type

#define IS_COPY_CONSTRUCTIBLE_COND(_TYPE) \
	std::is_copy_constructible_v<_TYPE>

#define IS_COPY_CONSTRUCTIBLE(_TYPE) \
	typename std::enable_if<IS_COPY_CONSTRUCTIBLE_COND(_TYPE)>::type

#define IS_MOVE_CONSTRUCTIBLE_COND(_TYPE) \
	std::is_move_constructible_v<_TYPE>

#define IS_MOVE_CONSTRUCTIBLE(_TYPE) \
	typename std::enable_if<IS_MOVE_CONSTRUCTIBLE_COND(_TYPE)>::type

#define IS_CONVERTIBLE_COND(_FROM,_TO) \
	std::is_convertible_v<_FROM,_TO>

#define IS_CONVERTIBLE(_FROM,_TO) \
	typename std::enable_if<IS_CONVERTIBLE_COND(_FROM,_TO)>::type

#define IS_ASSIGNABLE_COND(_TYPE,_ARG) \
	std::is_assignable_v<_TYPE,_ARG>

#define IS_ASSIGNABLE(_TYPE,_ARG) \
	typename std::enable_if<IS_ASSIGNABLE_COND(_TYPE,_ARG)>::type

#define IS_COPY_ASSIGNABLE_COND(_TYPE) \
	std::is_copy_assignable_v<_TYPE>

#define IS_COPY_ASSIGNABLE(_TYPE) \
	typename std::enable_if<IS_COPY_ASSIGNABLE_COND(_TYPE)>::type

#define IS_MOVE_ASSIGNABLE_COND(_TYPE) \
	std::is_move_assignable_v<_TYPE>

#define IS_MOVE_ASSIGNABLE(_TYPE) \
	typename std::enable_if<IS_MOVE_ASSIGNABLE_COND(_TYPE)>::type

#define IS_EQUALLY_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() == std::declval<_TYPE>())

#define IS_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() < std::declval<_TYPE>())

#define IS_INTEGRAL_COND(_TYPE) \
	std::is_integral<_TYPE>::value

#define IS_INTEGRAL(_TYPE) \
	typename std::enable_if<IS_INTEGRAL_COND(_TYPE)>::type

#define IS_FRACTIONAL_COND(_TYPE) \
	std::is_floating_point<_TYPE>::value

#define IS_FRACTIONAL(_TYPE) \
	typename std::enable_if<IS_FRACTIONAL_COND(_TYPE)>::type

#define IS_NUMERIC_COND(_TYPE) \
	std::is_arithmetic<_TYPE>::value

#define IS_NUMERIC(_TYPE) \
	typename std::enable_if<IS_NUMERIC_COND(_TYPE)>::type

#define TYPE_CONTAINS_SYMBOL(_TYPE,_SYMBOL) \
	typename _TYPE::_SYMBOL

#define IS_SCOPED_ENUM(_TYPE) \
	TYPE_CONTAINS_SYMBOL(_TYPE,scoped_enum_tag)

#define IS_TYPE_CONST_COND(_TYPE) \
	std::is_const<_TYPE>::value

#define IS_TYPE_CONST(_TYPE) \
	typename std::enable_if<IS_TYPE_CONST_COND(_TYPE)>::type

#define IS_TYPE_NON_CONST_COND(_TYPE) \
	(std::is_const<_TYPE>::value == false)

#define IS_TYPE_NON_CONST(_TYPE) \
	typename std::enable_if<IS_TYPE_NON_CONST_COND(_TYPE)>::type

#define IS_POINTER_COND(_TYPE) \
	std::is_pointer<_TYPE>::value

#define IS_POINTER(_TYPE) \
	typename std::enable_if<IS_POINTER_COND(_TYPE)>::type

#define IS_SIZED_TYPE_COND(_TYPE) \
	ddk::concepts::is_sized_type<_TYPE>

#define IS_SIZED_TYPE(_TYPE) \
	typename std::enable_if<IS_SIZED_TYPE_COND(_TYPE)>::type

#define DEDUCED_THIS_COND(_FROM,_TO) \
	IS_BASE_OF(_TO,_FROM)

#define DEDUCED_THIS(_FROM,_TO) \
	typename std::enable_if<DEDUCED_THIS_COND(_FROM,_TO)>::type

namespace ddk
{
namespace concepts
{

template<typename T, typename TT>
struct is_bindable_by_impl
{
private:
	static std::true_type resolve(const TT*);
	static std::false_type resolve(...);

public:
	static const bool value = decltype(resolve(std::declval<const T*>()))::value;
};

template<typename T, typename TT>
inline constexpr bool is_bindable_by = is_bindable_by_impl<T,TT>::value;

template<typename T>
struct is_sized_type_impl
{
private:
	template<typename TT>
	static std::true_type sized_type(TT&,const decltype(std::declval<T>().size())* = nullptr);
	template<typename TT>
	static std::false_type sized_type(const TT&,...);

public:
	static const bool value = decltype(sized_type(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_sized_type = is_sized_type_impl<T>::value;

template<typename T>
struct is_dimensioned_type_impl
{
private:
	template<typename TT>
	static std::true_type resolve(T&,decltype(std::declval<TT>().dimension())* = nullptr);
	template<template<typename,size_t...> typename TT,typename R,size_t ... Dims>
	static std::true_type resolve(const TT<R,Dims...>&);
	template<typename TT>
	static std::false_type resolve(const TT&,...);

public:
	static const bool value = decltype(resolve(std::declval<T&>()))::value;
};

template<typename T>
inline constexpr bool is_dimensioned_type = is_dimensioned_type_impl<T>::value;

}
}
