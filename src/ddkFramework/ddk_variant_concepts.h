#pragma once

#include <type_traits>

#define IS_VARIANT(_TYPE) \
	typename std::enable_if<ddk::concepts::is_variant_v<_TYPE>>::type*

namespace ddk
{
namespace concepts
{

template<typename T>
struct is_variant 
{
private:
	template<typename T>
	static std::true_type _is_variant(const T&,const typename T::variant_tag* = nullptr);
	static std::false_type _is_variant(...);

public:
	static const bool value = decltype(_is_variant(std::declval<T>()))::value;
};

template<typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

}
}