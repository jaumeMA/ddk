#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
class recursive_multi_map_balancer
{
public:
    typedef NestedBalancer<Map,Node> Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

template<template<typename,typename> class NestedBalancer,typename Map,typename Node>
class multi_map_balancer
{
public:
    typedef NestedBalancer<Map,Node> Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

}
}

#include "ddk_multi_map_balancer.inl"
#include "ddk_avl_map.h"

namespace ddk
{

template<template<typename,typename,template<typename>class> class Map>
struct multi_balancer;

template<template<typename,typename,template<typename>class> class Map>
struct recursive_balancer;

template<>
struct multi_balancer<avl_map>
{
	template<typename Map,typename Node>
	using Balancer = detail::multi_map_balancer<detail::avl_balancer,Map,Node>;
};

template<>
struct recursive_balancer<avl_map>
{
	template<typename Map,typename Node>
	using Balancer = detail::recursive_multi_map_balancer<detail::avl_balancer,Map,Node>;
};

}