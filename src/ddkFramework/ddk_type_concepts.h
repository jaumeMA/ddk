#pragma once

#include "ddk_template_helper.h"

#define IS_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>>::type

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_not_among_constructible_types<_TYPE,__VA_ARGS__>>::type

#define IS_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<std::is_base_of<_BASE,_TYPE>::value>::type

#define IS_NOT_BASE_OF(_BASE,_TYPE) \
	typename std::enable_if<std::is_base_of<_BASE,_TYPE>::value == false>::type

#define IS_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<std::is_same<_TYPE,__TYPE>::value>::type

#define IS_NOT_SAME_CLASS(_TYPE,__TYPE) \
	typename std::enable_if<std::is_same<_TYPE,__TYPE>::value==false>::type

#define IS_CLASS(_TYPE) \
	typename std::enable_if<std::is_class<_TYPE>::value>::type

#define IS_NOT_CLASS(_TYPE) \
	typename std::enable_if<std::is_class<_TYPE>::value == false>::type

#define IS_CONSTRUCTIBLE(_TYPE,...) \
	typename std::enable_if<std::is_constructible<_TYPE,__VA_ARGS__>::value>::type

#define IS_EQUALLY_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() == std::declval<_TYPE>())

#define IS_ASSIGNABLE(_TYPE) \
    decltype(std::declval<_TYPE>() = std::declval<_TYPE>())

#define IS_COMPARABLE(_TYPE) \
    decltype(std::declval<_TYPE>() < std::declval<_TYPE>())
