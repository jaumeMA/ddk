#pragma once

#include "ddk_template_helper.h"

#define IS_NUMBER_OF_ARGS(_NUM,...) \
    typename std::enable_if<ddk::mpl::get_num_types<__VA_ARGS__>() == _NUM>::type

#define IS_NUMBER_OF_ARGS_COND(_NUM,...) \
    (ddk::mpl::get_num_types<__VA_ARGS__>() == _NUM)

#define IS_NUMBER_OF_ARGS_GREATER(_NUM,...) \
    typename std::enable_if<ddk::mpl::get_num_types<__VA_ARGS__>() > _NUM>::type

#define IS_NUMBER_OF_ARGS_GREATER_COND(_NUM,...) \
    (ddk::mpl::get_num_types<__VA_ARGS__>() > _NUM)

#define IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(_NUM,...) \
    typename std::enable_if<ddk::mpl::get_num_types<__VA_ARGS__>() >= _NUM>::type

#define IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL_COND(_NUM,...) \
    (ddk::mpl::get_num_types<__VA_ARGS__>() >= _NUM)

#define IS_NUMBER_OF_ARGS_LESSER(_NUM,...) \
    typename std::enable_if<ddk::mpl::get_num_types<__VA_ARGS__>() < _NUM>::type

#define IS_NUMBER_OF_ARGS_LESSER_COND(_NUM,...) \
    (ddk::mpl::get_num_types<__VA_ARGS__>() <= _NUM)

#define IS_NUMBER_OF_ARGS_LESSER_OR_EQUAL(_NUM,...) \
    typename std::enable_if<ddk::mpl::get_num_types<__VA_ARGS__>() < _NUM>::type

#define IS_NUMBER_OF_ARGS_LESSER_OR_EQUAL_COND(_NUM,...) \
    (ddk::mpl::get_num_types<__VA_ARGS__>() <= _NUM)

#define IS_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>>::type

#define IS_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_not_among_constructible_types<_TYPE,__VA_ARGS__>>::type

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES_COND(_TYPE,...) \
	ddk::mpl::is_not_among_constructible_types<_TYPE,__VA_ARGS__>

#define IS_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<std::is_base_of<_BASE,_TYPE>::value>::type

#define IS_BASE_OF_COND(_BASE,_TYPE) \
	std::is_base_of<_BASE,_TYPE>::value

#define IS_NOT_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<std::is_base_of<_BASE,_TYPE>::value == false>::type

#define IS_NOT_BASE_OF_COND(_BASE,_TYPE) \
	(std::is_base_of<_BASE,_TYPE>::value == false)

#define IS_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<std::is_same<_TYPE,__TYPE>::value>::type

#define IS_SAME_CLASS_COND(_TYPE,__TYPE) \
	std::is_same<_TYPE,__TYPE>::value

#define IS_NOT_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<std::is_same<_TYPE,__TYPE>::value==false>::type

#define IS_NOT_SAME_CLASS_COND(_TYPE,__TYPE) \
	(std::is_same<_TYPE,__TYPE>::value==false)

#define IS_CLASS(_TYPE) \
	typename std::enable_if<std::is_class<_TYPE>::value>::type

#define IS_CLASS_COND(_TYPE) \
	std::is_class<_TYPE>::value

#define IS_NOT_CLASS(_TYPE) \
	typename std::enable_if<std::is_class<_TYPE>::value == false>::type

#define IS_NOT_CLASS_COND(_TYPE) \
	(std::is_class<_TYPE>::value == false)

#define IS_CONSTRUCTIBLE(_TYPE,...) \
	typename std::enable_if<std::is_constructible_v<_TYPE,__VA_ARGS__>>::type

#define IS_CONSTRUCTIBLE_COND(_TYPE,...) \
	std::is_constructible_v<_TYPE,__VA_ARGS__>

#define IS_CONVERTIBLE(_TYPE,...) \
	typename std::enable_if<std::is_convertible_v<_TYPE,__VA_ARGS__>>::type

#define IS_CONVERTIBLE_COND(_TYPE,...) \
	std::is_convertible_v<_TYPE,__VA_ARGS__>

#define IS_ASSIGNABLE(_TYPE,_ARG) \
	typename std::enable_if<std::is_assignable<_TYPE,_ARG>::value>::type

#define IS_ASSIGNABLE_COND(_TYPE,_ARG) \
	std::is_assignable<_TYPE,_ARG>::value

#define IS_EQUALLY_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() == std::declval<_TYPE>())

#define IS_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() < std::declval<_TYPE>())
