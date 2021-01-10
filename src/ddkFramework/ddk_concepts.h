#pragma once

#include "ddk_template_helper.h"
#include <type_traits>

#define TEMPLATE(...) template<__VA_ARGS__,
#define REQUIRES(...) typename = ddk::mpl::type_pack<__VA_ARGS__>>
#define REQUIRED(...) typename>
#define REQUIRES_COND(_Cond) \
	typename = typename std::enable_if<_Cond>::type*>
#define REQUIRED_COND(...) typename>