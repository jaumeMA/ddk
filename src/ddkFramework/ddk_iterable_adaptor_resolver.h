#pragma once

#include "ddk_iterable_adaptor.h"
#include "ddk_type_concepts.h"
#include "ddk_container_concepts.h"
#include "ddk_iterable_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_FORWARD_ITERABLE_COND(Iterable))
forward_iterable_adaptor<Iterable> iterable_adaptor_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE_COND(Iterable))
bidirectional_iterable_adaptor<Iterable> iterable_adaptor_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES_COND(IS_TYPE_NON_CONST_COND(Iterable) && DYNAMIC_SIZE_CONTAINER_COND(Iterable) && IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE_COND(Iterable))
random_access_iterable_adaptor<Iterable> iterable_adaptor_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_FORWARD_ITERABLE_COND(Iterable))
forward_iterable_adaptor<const Iterable> iterable_adaptor_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE_COND(Iterable))
bidirectional_iterable_adaptor<const Iterable> iterable_adaptor_resolver(Iterable&,...);

TEMPLATE(typename Iterable)
REQUIRES_COND((IS_TYPE_CONST_COND(Iterable) || (DYNAMIC_SIZE_CONTAINER_COND(Iterable) == false)) && IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE_COND(Iterable))
random_access_iterable_adaptor<const Iterable> iterable_adaptor_resolver(Iterable&,...);

template<typename Iterable>
using iterable_adaptor_correspondence = decltype(iterable_adaptor_resolver(std::declval<Iterable&>()));

template<typename Iterable>
typename iterable_adaptor<Iterable>::traits deduce_iterable_traits(const iterable_adaptor<Iterable>&);

}
    
template<typename Iterable>
struct iterable_adaptor : detail::iterable_adaptor_correspondence<Iterable>
{
    using detail::iterable_adaptor_correspondence<Iterable>::iterable_adaptor_correspondence;
};

template<typename Iterable>
using deduced_adaptor = decltype(deduce_adaptor(std::declval<Iterable>()));

}