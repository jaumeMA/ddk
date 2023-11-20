#pragma once

#include "ddk_iterable_action_defs.h"

#define IS_ITERABLE_ACTION_COND(_ACTION) \
    std::is_base_of<ddk::action_base,_ACTION>::value

#define IS_ITERABLE_ACTION(_ACTION) \
typename std::enable_if<IS_ITERABLE_ACTION_COND(_ACTION)>::type

#define IS_FILTERED_ACTION_COND(_ACTION) \
    (ddk::concepts::is_filtered_action<_ACTION>)

#define IS_FILTERED_ACTION(_ACTION) \
    typename std::enable_if<IS_FILTERED_ACTION_COND(_ACTION)>::type

namespace ddk
{
namespace concepts
{

template<typename Action>
struct _is_filtered_action;

template<typename Action>
struct _is_filtered_action<step_by_step_action<Action>>
{
    static const bool value = true;
};
template<typename Action>
struct _is_filtered_action
{
    static const bool value = false;
};

template<typename Action>
inline constexpr bool is_filtered_action = _is_filtered_action<Action>::value;

}
}