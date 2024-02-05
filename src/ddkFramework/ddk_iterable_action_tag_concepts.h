#pragma once

#include "ddk_iterable_action_recovery_tags.h"
#include "ddk_iterable_action_tags_template_helper.h"

#define ADAPTOR_SUPPORTS_TAGS_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_supports_actions<_ADAPTOR,__VA_ARGS__>

#define ADAPTOR_SUPPORTS_TAGS(_ADAPTOR,...) \
    typename std::enable_if<ADAPTOR_SUPPORTS_TAGS_COND(_ADAPTOR,__VA_ARGS__)>::type

#define ADAPTOR_TAGS_SUPPORTED_COND(_FROM_ADAPTOR,_TO_ADAPTOR) \
    ddk::concepts::adaptor_holds_adaptor_actions<_FROM_ADAPTOR,_TO_ADAPTOR>

#define ADAPTOR_TAGS_SUPPORTED(_FROM_ADAPTOR,_TO_ADAPTOR) \
    typename std::enable_if<ADAPTOR_TAGS_SUPPORTED_COND(_FROM_ADAPTOR,_TO_ADAPTOR)>::type

#define ADAPTOR_TAGS_EQUIVALENT_COND(_FROM_ADAPTOR,_TO_ADAPTOR) \
    ddk::concepts::adaptor_holds_adaptor_actions<_FROM_ADAPTOR,_TO_ADAPTOR> && ddk::concepts::adaptor_holds_adaptor_actions<_TO_ADAPTOR,_FROM_ADAPTOR>

#define ADAPTOR_TAGS_EQUIVALENT(_FROM_ADAPTOR,_TO_ADAPTOR) \
    typename std::enable_if<ADAPTOR_TAGS_EQUIVALENT_COND(_FROM_ADAPTOR,_TO_ADAPTOR)>::type

#define ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_holds_actions<_ADAPTOR,__VA_ARGS__>

#define ACTION_TAGS_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define ACTION_TAGS_NOT_SUPPORTED_COND(_ADAPTOR,...) \
    ( ddk::concepts::adaptor_holds_actions<_ADAPTOR,__VA_ARGS__> == false)

#define ACTION_TAGS_NOT_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ACTION_TAGS_NOT_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define ANY_ADAPTOR_ACTION_TAGS_SUPPORTED_COND(_FROM_ADAPTOR,_TO_ADAPTOR) \
    ddk::concepts::adaptor_holds_any_adaptor_actions<_FROM_ADAPTOR,_TO_ADAPTOR>

#define ANY_ADAPTOR_ACTION_TAGS_SUPPORTED(_FROM_ADAPTOR,_TO_ADAPTOR) \
    typename std::enable_if<ANY_ADAPTOR_ACTION_TAGS_SUPPORTED_COND(_FROM_ADAPTOR,_TO_ADAPTOR)>::type

#define ANY_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_holds_any_actions<_ADAPTOR,__VA_ARGS__>

#define ANY_ACTION_TAGS_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ANY_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION) \
    ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::aqcuire_callable_return_type<_FUNCTION>::type::tags_t)

#define TRANSFORMED_ACTION_TAGS_SUPPORTED(_ADAPTOR,_FUNCTION) \
    typename std::enable_if<TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION)>::type

#define ARE_ACTION_TAGS_CONTRAVARIANT_COND(_ACTION_TAG,_AACTION_TAG) \
    ddk::mpl::are_types_contravariant<_ACTION_TAG,_AACTION_TAG>

#define ARE_ACTION_TAGS_CONTRAVARIANT(_ACTION_TAG,_AACTION_TAG) \
    typename std::enable_if<ARE_ACTION_TAGS_CONTRAVARIANT_COND(_ACTION_TAG,_AACTION_TAG)>::type

#define IS_SINK_ACTION_COND(_ACTION_TAG) \
    ddk::mpl::is_sink_action_tag<_ACTION_TAG>

#define IS_SINK_ACTION(_ACTION_TAG) \
    typename std::enable_if<IS_SINK_ACTION_COND(_ACTION_TAG)>::type

namespace ddk
{
namespace concepts
{

template<typename,typename...>
struct _adaptor_supports_actions;

template<typename Adaptor,typename ... Tags>
struct _adaptor_supports_actions<Adaptor,mpl::type_pack<Tags...>>
{
private:
    template<typename T,typename TT,typename = decltype(std::declval<T>().perform_action(std::declval<TT>()))>
    static std::true_type resolve(T&&,TT&&);
    static std::false_type resolve(...);

public:
    static const bool value = (decltype(resolve(std::declval<Adaptor>(),std::declval<Tags&&>()))::value && ...);
};

template<typename Adaptor,typename Tag>
struct _adaptor_supports_actions<Adaptor,Tag>
{
private:
    template<typename T, typename TT, typename = decltype(std::declval<T>().perform_action(std::declval<TT>()))>
    static std::true_type resolve(T&&,TT&&);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<Adaptor>(),std::declval<Tag&&>()))::value;
};

template<typename Adaptor,typename ... ActionTags>
inline constexpr bool adaptor_supports_actions = (_adaptor_supports_actions<Adaptor,ActionTags>::value && ...);

template<typename ...>
struct _adaptor_holds_actions;

template<typename ... Tags,typename ... TTags>
struct _adaptor_holds_actions<mpl::type_pack<Tags...>,mpl::type_pack<TTags...>>
{
    static const bool value = ( mpl::is_among_constructible_types<Tags,TTags...> && ... );
};

template<typename Tag,typename ... Tags>
struct _adaptor_holds_actions<Tag,mpl::type_pack<Tags...>>
{
    static const bool value = mpl::is_among_constructible_types<Tag,Tags...>;
};

template<typename Adaptor, typename ... ActionTags>
inline constexpr bool adaptor_holds_actions = (_adaptor_holds_actions<ddk::mpl::remove_qualifiers<ActionTags>,ddk::detail::adaptor_tags<Adaptor>>::value && ...);

template<typename FromAdaptor,typename ToAdaptor>
inline constexpr bool adaptor_holds_adaptor_actions = (_adaptor_holds_actions<ddk::detail::adaptor_tags<FromAdaptor>,ddk::detail::adaptor_tags<ToAdaptor>>::value);

template<typename Tags,typename ... ActionTags>
inline constexpr bool __adaptor_holds_actions = (_adaptor_holds_actions<ddk::mpl::remove_qualifiers<ActionTags>,Tags>::value && ...);

template<typename Adaptor,typename ... ActionTags>
inline constexpr bool adaptor_holds_any_actions = (_adaptor_holds_actions<ddk::mpl::remove_qualifiers<ActionTags>,ddk::detail::adaptor_tags<Adaptor>>::value || ...);

template<typename FromAdaptor,typename ToAdaptor>
inline constexpr bool adaptor_holds_any_adaptor_actions = (_adaptor_holds_actions<ddk::detail::adaptor_tags<FromAdaptor>,ddk::detail::adaptor_tags<ToAdaptor>>::value);

}
}