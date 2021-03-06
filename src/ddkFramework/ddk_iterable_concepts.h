#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterator_concepts.h"

#define IS_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value>::type

#define IS_BASE_OF_ITERABLE_COND(_TYPE) \
	std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value

#define IS_NOT_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false>::type

#define IS_NOT_BASE_OF_ITERABLE_COND(_TYPE) \
	(std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false)

#define IS_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<ddk::concepts::is_non_const_iterable_v<_TYPE> == false>::type

#define IS_CONST_ITERABLE_COND(_TYPE) \
    (ddk::concepts::is_non_const_iterable_v<_TYPE> == false)

#define IS_NON_CONST_ITERABLE(_TYPE) \
    typename std::enable_if<ddk::concepts::is_non_const_iterable_v<_TYPE>>::type

#define IS_NON_CONST_ITERABLE_COND(_TYPE) \
    ddk::concepts::is_non_const_iterable_v<_TYPE>

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
inline constexpr bool is_non_const_iterable_v = is_non_const_iterable<typename std::remove_reference<T>::type>::value;

template<typename IterableValue,typename T,size_t ... Indexs>
typename mpl::static_if<mpl::holds_type_for_some_type<std::is_constructible,IterableValue&&,typename T::template nth_type<Indexs>...>(),std::true_type,std::false_type>::type _resolve_iterable_valued_function(const mpl::sequence<Indexs...>&);
template<typename Iterable,typename Function,typename T = mpl::aqcuire_callable_args_type<Function>::type>
decltype(_resolve_iterable_valued_function<typename Iterable::iterable_value,T>(typename mpl::make_sequence<0,T::size()>::type{})) resolve_iterable_valued_function(const Iterable&,Function&);
template<typename Iterable,typename Function,typename = decltype(std::declval<Function>()(std::declval<typename Iterable::iterable_value>()))>
std::true_type resolve_iterable_valued_function(const Iterable&,const Function&);
template<typename ... T>
std::false_type resolve_iterable_valued_function(const T& ...);

template<typename Iterable,typename Function>
inline constexpr bool is_iterable_valued_function = decltype(resolve_iterable_valued_function(std::declval<Iterable>(),std::declval<mpl::remove_qualifiers<Function>&>()))::value;

}
}
