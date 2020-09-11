#pragma once

#include "ddk_intersection_function.h"
#include "ddk_function.h"

#define DEFINE_HIGHER_ORDER_UNARY_FUNCTION(NAME,OP) \
template<typename Return> \
struct NAME##_unary_op\
{ \
    typedef decltype((OP std::declval<const Return>())) inner_return; \
    NAME##_unary_op() = default; \
    inner_return operator()(const Return& i_value) const \
    { \
        return OP i_value; \
    } \
}; \
template<typename ...> \
class NAME##_callable_t; \
 \
template<typename Return, typename ... Types> \
class NAME##_callable_t<Return(Types...)> : public ddk::detail::intersection_function<NAME##_unary_op<Return>,ddk::function<Return(Types...)>> \
{ \
	typedef ddk::detail::intersection_function<NAME##_unary_op<Return>,ddk::function<Return(Types...)>> base_t; \
public: \
    NAME##_callable_t(const ddk::function<Return(Types...)>& i_func) \
    : base_t(NAME##_unary_op<Return>{},i_func) \
    {} \
}; \
template<typename Return, typename ... Types> \
ddk::function<Return(Types...)> operator OP(const ddk::function<Return(Types...)>& i_function) \
{ \
    return ddk::NAME##_callable_t<Return(Types...)>(i_function); \
}

#define DEFINE_HIGHER_ORDER_BINARY_FUNCTION(NAME,OP) \
template<typename Return> \
struct NAME##_binary_op\
{ \
    typedef decltype((std::declval<const Return>() OP std::declval<const Return>())) inner_return; \
    NAME##_binary_op() = default; \
    inner_return operator()(const Return& i_lhs, const Return& i_rhs) const \
    { \
        return i_lhs OP i_rhs; \
    } \
}; \
template<typename...> \
class NAME##_callable_t; \
 \
template<typename Return, typename ... Types> \
class NAME##_callable_t<Return(Types...)> : public ddk::detail::intersection_function<NAME##_binary_op<Return>,ddk::function<Return(Types...)>,ddk::function<Return(Types...)>> \
{ \
	typedef ddk::detail::intersection_function<NAME##_binary_op<Return>,ddk::function<Return(Types...)>,ddk::function<Return(Types...)>> base_t; \
public: \
    NAME##_callable_t(const ddk::function<Return(Types...)>& i_lhs, const ddk::function<Return(Types...)>& i_rhs) \
    : base_t(NAME##_binary_op<Return>{},i_lhs,i_rhs) \
    {} \
}; \
template<typename Return, typename ... Types> \
ddk::function<Return(Types...)> operator OP(const ddk::function<Return(Types...)>& i_lhs, const ddk::function<Return(Types...)>& i_rhs) \
{ \
    if(i_lhs != nullptr && i_rhs != nullptr) \
    { \
		ddk::NAME##_callable_t<Return(Types...)>{i_lhs, i_rhs}; \
        return nullptr; \
    } \
    else if(i_lhs != nullptr) \
    { \
        return i_lhs; \
    } \
    else \
    { \
        return i_rhs; \
    } \
} \

namespace ddk
{

DEFINE_HIGHER_ORDER_UNARY_FUNCTION(not,!)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(and,&&)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(or,||)
DEFINE_HIGHER_ORDER_BINARY_FUNCTION(xor,^)

}
