#pragma once

#define HAS_ITERATOR_DEFINED_COND(_TYPE) \
    ddk::concepts::has_iterator_defined_v<_TYPE>

#define HAS_ITERATOR_DEFINED(_TYPE) \
    typename std::enable_if<HAS_ITERATOR_DEFINED_COND(_TYPE)>::type

#define IS_CONST_ITERATOR_COND(_TYPE) \
    ddk::concepts::is_const_iterator_defined_v<_TYPE>

#define IS_CONST_ITERATOR(_TYPE) \
    typename std::enable_if<IS_CONST_ITERATOR_COND(_TYPE)>::type

#define IS_NON_CONST_ITERATOR_COND(_TYPE) \
    (ddk::concepts::is_const_iterator_defined_v<_TYPE> == false)

#define IS_NON_CONST_ITERATOR(_TYPE) \
    typename std::enable_if<IS_NON_CONST_ITERATOR_COND(_TYPE)>::type

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
inline constexpr bool is_const_iterator_defined_v = mpl::is_const<decltype(std::declval<T>().operator*())>;

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

template<typename,typename>
struct iterator_action_support_impl;

template<typename Iterator>
struct iterator_action_support_impl<Iterator,forward_action_tag>
{
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(std::declval<T>()++)*);
    static std::false_type resolve(...);

public:
    typedef typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<forward_action_tag>,mpl::empty_type_pack>::type type;
};

template<typename Iterator>
struct iterator_action_support_impl<Iterator,backward_action_tag>
{
private:
    template<typename T>
    static std::true_type resolve(T&, const decltype(std::declval<T>()--)*);
    static std::false_type resolve(...);

public:
    typedef typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<backward_action_tag>,mpl::empty_type_pack>::type type;
};

template<typename Iterator>
struct iterator_action_support_impl<Iterator,displace_action_tag>
{
private:
    template<typename T>
    static std::true_type resolve(T&,const decltype(std::declval<T>() + 0)*);
    static std::false_type resolve(...);

public:
    typedef typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<displace_action_tag>,mpl::empty_type_pack>::type type;
};

template<typename Iterator>
struct iterator_action_support_impl<Iterator,mpl::template_class_holder<agnostic_sink_action_tag>>
{
private:
    template<typename T>
    static std::true_type resolve(T&, const decltype(&*std::declval<T>())*);
    static std::false_type resolve(...);

public:
    typedef typename mpl::which_type<decltype(resolve(std::declval<Iterator&>(),nullptr))::value,mpl::type_pack<agnostic_sink_action_tag<typename Iterator::reference>>,mpl::empty_type_pack>::type type;
};

template<typename Iterator,typename Action>
using iterator_action_support = typename iterator_action_support_impl<Iterator,Action>::type;

}
}
