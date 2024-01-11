#pragma once

#include "ddk_iterable_action_defs.h"
#include "ddk_iterable_action_tag_concepts.h"

#define IS_ITERABLE_ACTION_COND(_ACTION) \
    std::is_base_of<ddk::action_base,ddk::mpl::remove_qualifiers<_ACTION>>::value

#define IS_ITERABLE_ACTION(_ACTION) \
typename std::enable_if<IS_ITERABLE_ACTION_COND(_ACTION)>::type

#define ACTION_SUPPORTED_COND(_ADAPTOR,_ACTION) \
    ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::remove_qualifiers<_ACTION>::tags_t)

#define ACTION_SUPPORTED(_ADAPTOR,_ACTION) \
    typename std::enable_if<ACTION_SUPPORTED_COND(_ADAPTOR,_ACTION)>::type

#define ACTION_NOT_SUPPORTED_COND(_ADAPTOR,_ACTION) \
    ACTION_TAGS_NOT_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::remove_qualifiers<_ACTION>::tags_t)

#define ACTION_NOT_SUPPORTED(_ADAPTOR,_ACTION) \
    typename std::enable_if<ACTION_NOT_SUPPORTED_COND(_ADAPTOR,_ACTION)>::type

#define TRANSFORMED_ACTION_SUPPORTED_COND(_ADAPTOR,_FUNCTION) \
    ACTION_TAGS_SUPPORTED_COND(_ADAPTOR,typename ddk::mpl::aqcuire_callable_return_type<_FUNCTION>::type::tags_t)

#define TRANSFORMED_ACTION_SUPPORTED(_ADAPTOR,_FUNCTION) \
    typename std::enable_if<TRANSFORMED_ACTION_SUPPORTED_COND(_ADAPTOR,_FUNCTION)>::type

namespace ddk
{
namespace concepts
{

}
}