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
struct is_iterable_deducible_type_impl
{
private:
    template<typename TT>
    static std::true_type resolve(TT&&,const ddk::deduced_iterable<TT&&>* = nullptr);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<T&&>()))::value;
};

template<typename T>
inline constexpr bool is_iterable_deducible_type = is_iterable_deducible_type_impl<T>::value;

}
}
