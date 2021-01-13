#pragma once

#include "ddk_template_helper.h"
#include <type_traits>

#define IS_VARIANT(_TYPE) \
	typename std::enable_if<ddk::concepts::is_variant_v<_TYPE>>::type

#define IS_STATIC_VISITOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_static_visitor_v<_TYPE>>::type

#define IS_BASE_OF_STATIC_VISITOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_base_of_static_visitor_v<_TYPE>>::type

namespace ddk
{

template<typename>
class static_visitor;

namespace concepts
{

template<typename T>
struct is_variant
{
private:
    template<typename ... TT>
    static std::true_type resolve(const variant<TT...>&);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<T>()))::value;
};

template<typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template<typename>
struct is_static_visitor;

template<typename T>
struct is_static_visitor<static_visitor<T>> : public std::true_type
{
};
template<typename T>
struct is_static_visitor : public std::false_type
{
};

template<typename T>
inline constexpr bool is_static_visitor_v = is_static_visitor<T>::value;

template<typename T>
struct is_base_of_static_visitor
{
private:
    template<typename TT>
    static std::is_base_of<static_visitor<typename TT::return_type>,TT> resolve(const TT&);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<T>()))::value;
};

template<typename T>
inline constexpr bool is_base_of_static_visitor_v = is_base_of_static_visitor<T>::value;

}
}
