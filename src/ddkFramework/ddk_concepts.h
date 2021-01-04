#pragma once

#include <tuple>

#define TEMPLATE(...) template<__VA_ARGS__,
#define REQUIRES(...) typename = std::tuple<__VA_ARGS__>>
#define REQUIRED(...) typename>
#define REQUIRES_COND(_Cond) \
	typename = typename std::enable_if<_Cond>::type*>
#define REQUIRED_COND(...) typename>