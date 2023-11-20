#pragma once

#include "ddk_iterable_action_tags.h"

#define ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,...) \
    ddk::concepts::adaptor_holds_actions<ddk::mpl::remove_qualifiers<_ADAPTOR>,__VA_ARGS__>

#define ACTION_TAGS_SUPPORTED(_ADAPTOR,...) \
    typename std::enable_if<ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,__VA_ARGS__)>::type

#define TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION) \
    ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::aqcuire_callable_return_type<_FUNCTION>::type::tags_t)

#define TRANSFORMED_ACTION_TAGS_SUPPORTED(_ADAPTOR,_FUNCTION) \
    typename std::enable_if<TRANSFORMED_ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,_FUNCTION)>::type

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
inline constexpr bool adaptor_holds_actions = (_adaptor_holds_actions< ddk::mpl::remove_qualifiers<ActionTags>,typename Adaptor::tags_t>::value && ...);

}
}