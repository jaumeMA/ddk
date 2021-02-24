#pragma once

#include "ddk_template_helper.h"
#include <type_traits>

#define TEMPLATE(...) template<__VA_ARGS__,
#define REQUIRES(...) typename = ddk::mpl::type_pack<__VA_ARGS__>>
#define REQUIRED(...) typename>
#define REQUIRES_COND(...) \
	typename = ddk::mpl::type_pack<typename std::enable_if<ddk::mpl::evaluate_and<__VA_ARGS__>()>::type>>
#define REQUIRED_COND(...) typename>