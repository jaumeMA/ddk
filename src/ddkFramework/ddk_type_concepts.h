#pragma once

#include "ddk_template_helper.h"

#define IS_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>::value>::type*

#define IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(_TYPE,...) \
	typename std::enable_if<ddk::mpl::is_among_constructible_types<_TYPE,__VA_ARGS__>::value==false>::type*

#define IS_CLASS(_TYPE) \
	typename std::enable_if<std::is_class<_TYPE>::value>::type*