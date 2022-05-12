#pragma once

#include "ddk_iterable_type.h"
#include "ddk_iterable_concepts.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_FORWARD_ITERABLE_COND(Iterable))
forward_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE_COND(Iterable))
bidirectional_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE_COND(Iterable))
random_access_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_FORWARD_ITERABLE_COND(Iterable))
const_forward_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE_COND(Iterable))
const_bidirectional_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE_COND(Iterable))
const_random_access_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

template<typename Iterable>
using iterable_type_correspondence = decltype(iterable_type_correspondence_resolver(std::declval<Iterable&>()));

}
}
