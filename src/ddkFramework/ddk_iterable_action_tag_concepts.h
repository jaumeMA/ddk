#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_template_helper.h"

#define ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_holds_actions<ddk::mpl::remove_qualifiers<_ADAPTOR>,__VA_ARGS__>

#define ACTION_TAGS_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define ACTION_TAGS_NOT_SUPPORTED_COND(_ADAPTOR,...) \
    ( ddk::concepts::adaptor_holds_actions<ddk::mpl::remove_qualifiers<_ADAPTOR>,__VA_ARGS__> == false)

#define ACTION_TAGS_NOT_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ACTION_TAGS_NOT_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define ANY_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_holds_any_actions<ddk::mpl::remove_qualifiers<_ADAPTOR>,__VA_ARGS__>

#define ANY_ACTION_TAGS_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ANY_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION) \
    ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::aqcuire_callable_return_type<_FUNCTION>::type::tags_t)

#define TRANSFORMED_ACTION_TAGS_SUPPORTED(_ADAPTOR,_FUNCTION) \
    typename std::enable_if<TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION)>::type

#define IS_ACTION_TAG_RECOVERABLE_COND(_ACTION_TAG) \
    ddk::concepts::action_tag_recoverable<_ACTION_TAG>

#define IS_ACTION_TAG_RECOVERABLE(_ACTION_TAG) \
    typename std::enable_if<IS_ACTION_TAG_RECOVERABLE_COND(_ACTION_TAG)>::type

namespace ddk
{
namespace concepts
{

template<typename ...>
struct _adaptor_holds_actions;

template<typename ... Tags,typename ... TTags>
struct _adaptor_holds_actions<mpl::type_pack<Tags...>,mpl::type_pack<TTags...>>
{
    static const bool value = ( mpl::is_among_types<Tags,TTags...> && ...);
};

template<typename Tag,typename ... Tags>
struct _adaptor_holds_actions<Tag,mpl::type_pack<Tags...>>
{
    static const bool value = mpl::is_among_types<Tag,Tags...>;
};

template<typename Adaptor, typename ... ActionTags>
inline constexpr bool adaptor_holds_actions = (_adaptor_holds_actions<ddk::mpl::remove_qualifiers<ActionTags>,typename Adaptor::tags_t>::value && ...);

template<typename Adaptor,typename ... ActionTags>
inline constexpr bool adaptor_holds_any_actions = (_adaptor_holds_actions<ddk::mpl::remove_qualifiers<ActionTags>,typename Adaptor::tags_t>::value || ...);

template<typename Tag>
struct _action_tag_recoverable
{
private:
    template<typename T>
    static std::true_type resolve(const T&, typename T::recovery_tag* = nullptr);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<Tag>()))::value;
};

template<typename ActionTag>
inline constexpr bool action_tag_recoverable = _action_tag_recoverable<ActionTag>::value;

}
}