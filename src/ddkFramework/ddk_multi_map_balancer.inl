
#include "ddk_map_balancer.h"

namespace ddk
{
namespace detail
{

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
unique_reference_wrapper<Node> recursive_multi_map_balancer<NestedBalancer,Map,Node>::balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    typedef typename Node::node_value_t::value_t value_t;

    if(lent_pointer_wrapper<value_t> fromMapRef =  static_lent_cast<value_t>(fromMap))
    {
        fromNode->get_value().second.set_holder(promote_to_ref(fromMapRef));
        fromNode->get_value().second.set_node_location(fromNode);

        return NestedBalancer<Map,Node>::balance(fromMap,fromNode);
    }
    else
    {
        return std::move(fromNode);
    }
}

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
unique_reference_wrapper<Node> multi_map_balancer<NestedBalancer,Map,Node>::balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode)
{
    fromNode->get_value().second.set_holder(nullptr);
    fromNode->get_value().second.set_node_location(fromNode);

    return NestedBalancer<Map,Node>::balance(fromMap,fromNode);
}

}
}