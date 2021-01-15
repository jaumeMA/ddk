#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterator_concepts.h"

#define IS_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value>::type

#define IS_NOT_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false>::type

#define IS_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<ddk::concepts::is_non_const_iterable_v<_TYPE> == false>::type

#define IS_NON_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<ddk::concepts::is_non_const_iterable_v<_TYPE>>::type

#define IS_FORWARD_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_FORWARD_ITERATOR(typename _TYPE::iterator)

#define IS_BIDIRECTIONAL_ITERABLE(_TYPE) \
    IS_FORWARD_ITERABLE(_TYPE),IS_BIDIRECTIONAL_ITERATOR(typename _TYPE::iterator)

#define IS_RANDOM_ACCESS_ITERABLE(_TYPE) \
    IS_BIDIRECTIONAL_ITERABLE(_TYPE),IS_RANDOM_ACCESS_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_FORWARD_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_EXCLUSIVE_FORWARD_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE), IS_EXCLUSIVE_BIDIRECTIONAL_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_EXCLUSIVE_RANDOM_ACCESS_ITERATOR(typename _TYPE::iterator)


namespace ddk
{
namespace concepts
{

template<typename T>
struct is_non_const_iterable
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().insert(std::declval<typename TT::const_iterator>(),std::declval<typename TT::value_type>()))>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_non_const_iterable_v = is_non_const_iterable<T>::value;

}
}
