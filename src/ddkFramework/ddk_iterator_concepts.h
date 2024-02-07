#pragma once

#define HAS_ITERATOR_DEFINED_COND(_TYPE) \
    ddk::concepts::has_iterator_defined_v<_TYPE>

#define HAS_ITERATOR_DEFINED(_TYPE) \
    typename std::enable_if<HAS_ITERATOR_DEFINED_COND(_TYPE)>::type

#define IS_FORWARD_ITERATOR_COND(_TYPE) \
    ddk::concepts::is_forward_iterator_v<_TYPE>

#define IS_FORWARD_ITERATOR(_TYPE) \
    typename std::enable_if<IS_FORWARD_ITERATOR_COND(_TYPE)>::type

#define IS_BACKWARD_ITERATOR_COND(_TYPE) \
    ddk::concepts::is_backward_iterator_v<_TYPE>

#define IS_BACKWARD_ITERATOR(_TYPE) \
    typename std::enable_if<IS_BACKWARD_ITERATOR_COND(_TYPE)>::type

#define IS_BIDIRECTIONAL_ITERATOR_COND(_TYPE) \
    (IS_FORWARD_ITERATOR_COND(_TYPE) && IS_BACKWARD_ITERATOR_COND(_TYPE))

#define IS_BIDIRECTIONAL_ITERATOR(_TYPE) \
    typename std::enable_if<IS_BIDIRECTIONAL_ITERATOR_COND(_TYPE)>::type

#define IS_RANDOM_ACCESS_ITERATOR_COND(_TYPE) \
    (IS_BIDIRECTIONAL_ITERATOR_COND(_TYPE) && ddk::concepts::is_random_access_iterator_v<_TYPE>)

#define IS_RANDOM_ACCESS_ITERATOR(_TYPE) \
    typename std::enable_if<IS_RANDOM_ACCESS_ITERATOR_COND(_TYPE)>::type

#define IS_EXCLUSIVE_FORWARD_ITERATOR_COND(_TYPE) \
    (IS_FORWARD_ITERATOR_COND(_TYPE) && (IS_BACKWARD_ITERATOR_COND(_TYPE) == false))

#define IS_EXCLUSIVE_FORWARD_ITERATOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_FORWARD_ITERATOR_COND(_TYPE)>::type

#define IS_EXCLUSIVE_BIDIRECTIONAL_ITERATOR_COND(_TYPE) \
    (IS_FORWARD_ITERATOR_COND(_TYPE) && IS_BACKWARD_ITERATOR_COND(_TYPE) && (IS_RANDOM_ACCESS_ITERATOR_COND(_TYPE) == false))

#define IS_EXCLUSIVE_BIDIRECTIONAL_ITERATOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_BIDIRECTIONAL_ITERATOR_COND(_TYPE)>::type

#define IS_EXCLUSIVE_RANDOM_ACCESS_ITERATOR_COND(_TYPE) \
    (IS_FORWARD_ITERATOR_COND(_TYPE) && IS_BACKWARD_ITERATOR_COND(_TYPE) && IS_RANDOM_ACCESS_ITERATOR_COND(_TYPE))

#define IS_EXCLUSIVE_RANDOM_ACCESS_ITERATOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_RANDOM_ACCESS_ITERATOR_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename T>
struct has_iterator_defined
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename TT::iterator*);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool has_iterator_defined_v = has_iterator_defined<mpl::remove_qualifiers<T>>::value;

template<typename T>
struct is_forward_iterator
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>()++)>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_forward_iterator_v = is_forward_iterator<mpl::remove_qualifiers<T>>::value;

template<typename T>
struct is_backward_iterator
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>()--)>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_backward_iterator_v = is_backward_iterator<mpl::remove_qualifiers<T>>::value;

template<typename T>
struct is_random_access_iterator
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>()+0)>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_random_access_iterator_v = is_random_access_iterator<mpl::remove_qualifiers<T>>::value;

}
}
