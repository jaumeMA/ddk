#pragma once

#define IS_DEDUCIBLE_ITERABLE_TYPE_COND(_TYPE) \
    ddk::concepts::is_iterable_deducible_type<ddk::mpl::remove_qualifiers<_TYPE>>

#define IS_DEDUCIBLE_ITERABLE_TYPE(_TYPE) \
    typename std::enable_if<IS_DEDUCIBLE_ITERABLE_TYPE_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename T>
inline constexpr bool is_iterable_deducible_type = (std::is_base_of<detail::none_t,ddk::iterable_adaptor<T>>::value == false);

}
}