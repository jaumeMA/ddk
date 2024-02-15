#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_type_concepts.h"
#include "ddk_iterable_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_FORWARD_NON_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::reference>,begin_action_tag,forward_action_tag,add_action_tag<typename Iterable::value_type>,remove_action_tag> iterable_action_tag_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_BIDIRECTIONAL_NON_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,add_action_tag<typename Iterable::value_type>,remove_action_tag> iterable_action_tag_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_RANDOM_ACCESS_NON_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag,add_action_tag<typename Iterable::value_type>,remove_action_tag> iterable_action_tag_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_FORWARD_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::const_reference>,begin_action_tag,forward_action_tag> iterable_action_tag_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_BIDIRECTIONAL_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::const_reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag> iterable_action_tag_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES(IS_EXCLUSIVE_RANDOM_ACCESS_CONST_ITERABLE(Iterable))
mpl::type_pack<agnostic_sink_action_tag<typename Iterable::const_reference>,begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag,displace_action_tag> iterable_action_tag_resolver(Iterable&,...);

mpl::empty_type_pack iterable_action_tag_resolver(...);

}
}