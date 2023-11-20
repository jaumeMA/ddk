#pragma once

#define IS_CONST_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_non_const_adaptor_v<_TYPE> == false

#define IS_CONST_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_ADAPTOR_COND(_TYPE)>::type

#define IS_NON_CONST_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_non_const_adaptor_v<_TYPE>

#define IS_NON_CONST_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_ADAPTOR_COND(_TYPE)>::type

#define IS_FORWARD_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_forward_adaptor_v<_TYPE>

#define IS_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_FORWARD_ADAPTOR_COND(_TYPE)>::type

#define IS_CONST_FORWARD_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE) && IS_FORWARD_ADAPTOR_COND(_TYPE)

#define IS_CONST_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_FORWARD_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_FORWARD_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE) && IS_FORWARD_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_FORWARD_ADAPTOR_COND(_TYPE)>::type    

#define IS_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    IS_FORWARD_ADAPTOR_COND(_TYPE) && ddk::concepts::is_backward_adaptor_v<_TYPE>

#define IS_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_CONST_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE) && IS_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)

#define IS_CONST_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE) && IS_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    IS_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) && ddk::concepts::is_random_access_adaptor_v<_TYPE>

#define IS_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

#define IS_CONST_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE) && IS_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)

#define IS_CONST_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE) && IS_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

#define IS_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_forward_adaptor_v<_TYPE> && ddk::concepts::is_backward_adaptor_v<_TYPE> == false

#define IS_EXCLUSIVE_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE)>::type

#define IS_CONST_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE), IS_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE)

#define IS_CONST_EXCLUSIVE_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE), IS_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_EXCLUSIVE_FORWARD_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_EXCLUSIVE_FORWARD_ADAPTOR_COND(_TYPE)>::type    

#define IS_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_forward_adaptor_v<_TYPE> && \
    ddk::concepts::is_backward_adaptor_v<_TYPE> && \
    ddk::concepts::is_random_access_adaptor_v<_TYPE> == false

#define IS_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE) && IS_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)

#define IS_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE) && IS_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR_COND(_TYPE)>::type    

#define IS_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    ddk::concepts::is_forward_adaptor_v<_TYPE> && \
    ddk::concepts::is_backward_adaptor_v<_TYPE> && \
    ddk::concepts::is_random_access_adaptor_v<_TYPE>

#define IS_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

#define IS_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    IS_CONST_ADAPTOR_COND(_TYPE) && IS_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)

#define IS_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

#define IS_NON_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE) \
    IS_NON_CONST_ADAPTOR_COND(_TYPE) && IS_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)

#define IS_NON_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR_COND(_TYPE)>::type    

namespace ddk
{
namespace concepts
{

class adaptor_value_receiver
{
public:
    template<typename ... T>
    void operator()(T&& ...) const;
};

template<typename T>
struct is_non_const_adaptor
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().erase_value())>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_non_const_adaptor_v = is_non_const_adaptor<T>::value;

template<typename T>
struct is_forward_adaptor
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().forward_next_value())>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_forward_adaptor_v = is_forward_adaptor<T>::value;

template<typename T>
struct is_backward_adaptor
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().forward_prev_value())>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_backward_adaptor_v = is_backward_adaptor<T>::value;

template<typename T>
struct is_random_access_adaptor
{
private:
    template<typename TT>
    static std::true_type resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().forward_shift_value(0))>::type);
    template<typename TT>
    static std::false_type resolve(TT&, ...);

public:
    static const bool value = decltype(resolve(std::declval<T&>(),nullptr))::value;
};

template<typename T>
inline constexpr bool is_random_access_adaptor_v = is_random_access_adaptor<T>::value;

}
}
