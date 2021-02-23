#pragma once

#include "ddk_function.h"

#define DEFINE_HIGHER_ORDER_UNARY_FUNCTION(NAME,OP) \
\
template<typename Return> \
using inner_unary_return = decltype((OP std::declval<const Return>())); \
\
template<typename Return> \
struct NAME##_unary_op\
{ \
    NAME##_unary_op() = default; \
    inner_unary_return<Return> operator()(const Return& i_value) const \
    { \
        return OP i_value; \
    } \
}; \
template<typename Return, typename ... Types> \
ddk::function<inner_unary_return<Return>(Types...)> operator OP(const ddk::function<Return(Types...)>& i_function) \
{ \
    return ddk::make_composition(ddk::function<inner_unary_return<Return>(const Return&)>(NAME##_unary_op<Return>{}),i_function); \
}

#define DEFINE_HIGHER_ORDER_BINARY_FUNCTION(NAME,OP) \
template<typename ReturnA, typename ReturnB> \
struct NAME##_binary_op\
{ \
    typedef decltype(std::declval<ReturnA>() OP std::declval<ReturnB>()) inner_binary_return; \
    NAME##_binary_op() = default; \
    inner_binary_return operator()(const ReturnA& i_lhs, const ReturnB& i_rhs) const \
    { \
        return i_lhs OP i_rhs; \
    } \
}; \
template<typename ReturnA, typename ReturnB, typename ... Types, typename AllocatorA, typename AllocatorB> \
ddk::function<decltype(std::declval<ReturnA>() OP std::declval<ReturnB>())(Types...)> operator OP(const ddk::function<ReturnA(Types...),AllocatorA>& i_lhs, const ddk::function<ReturnB(Types...),AllocatorB>& i_rhs) \
{ \
    typedef decltype(std::declval<ReturnA>() OP std::declval<ReturnB>()) inner_binary_return; \
    if(i_lhs != nullptr && i_rhs != nullptr) \
    { \
        return ddk::make_composition(ddk::function<inner_binary_return(const ReturnA&,const ReturnB&)>(NAME##_binary_op<ReturnA,ReturnB>{}),ddk::function<ddk::function_arguments<ReturnA,ReturnB>(Types...)>(ddk::make_intersection(i_lhs,i_rhs))); \
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
