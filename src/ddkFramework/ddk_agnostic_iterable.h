#pragma once

#include "ddk_function_template_helper.h"
#include "ddk_type_erasure_iterable_impl.h"

namespace ddk
{
namespace detail
{

template<typename T,typename Tags, typename ConstTags>
using owned_iterable = iterable<type_erasure_iterable_impl<iterable_traits<iterable_by_type_adaptor<T,Tags,ConstTags>>>>;

}

template<typename T>
using const_forward_iterable = detail::owned_iterable<T,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,forward_action_tag>>;

template<typename T>
using forward_iterable = detail::owned_iterable<T,mpl::type_pack<agnostic_sink_action_tag<T&>,add_action_tag<T>,remove_action_tag>,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,forward_action_tag>>;

template<typename T>
using const_backward_iterable = detail::owned_iterable<T,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const T&>,last_action_tag,forward_action_tag>>;

template<typename T>
using backward_iterable = detail::owned_iterable<T,mpl::type_pack<agnostic_sink_action_tag<T&>,add_action_tag<T>,remove_action_tag>,mpl::type_pack<agnostic_sink_action_tag<const T&>,last_action_tag,forward_action_tag>>;

template<typename T>
using const_bidirectional_iterable = detail::owned_iterable<T,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag>>;

template<typename T>
using bidirectional_iterable = detail::owned_iterable<T,mpl::type_pack<agnostic_sink_action_tag<T&>,add_action_tag<T>,remove_action_tag>,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag>>;

template<typename T>
using const_random_access_iterable = detail::owned_iterable<T,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>>;

template<typename T>
using random_access_iterable = detail::owned_iterable<T,mpl::type_pack<agnostic_sink_action_tag<T&>,add_action_tag<T>,remove_action_tag>,mpl::type_pack<agnostic_sink_action_tag<const T&>,begin_action_tag,last_action_tag,forward_action_tag,backward_action_tag,displace_action_tag>>;

}