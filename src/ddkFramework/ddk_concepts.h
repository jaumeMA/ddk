#pragma once

#include <tuple>

#define TEMPLATE(...) template<__VA_ARGS__,
#define REQUIRES(...) typename = std::tuple<__VA_ARGS__>>
#define REQUIRED(...) typename>