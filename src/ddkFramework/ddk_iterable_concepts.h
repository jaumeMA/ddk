#pragma once

#include "ddk_iterable_interface.h"

#define IS_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value>::type

#define IS_NOT_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false>::type