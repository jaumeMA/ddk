#include "ddk_iterable_action_resolver.h"

namespace ddk
{
namespace iter
{
namespace detail
{

reversable_action_resolver::reversable_action_resolver(bool i_reversed)
: m_reversed(i_reversed)
{
}

}

const detail::reversable_action_resolver forward_order = detail::reversable_action_resolver(false);
const detail::reversable_action_resolver reverse_order = detail::reversable_action_resolver(true);

}
}