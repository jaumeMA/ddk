#include "ddk_iterable.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace
{

const detail::__numbers_iterable integers_container = detail::__numbers_iterable([](const int& i_value) { return i_value + 1; });
const detail::__numbers_iterable odd_integers_container = detail::__numbers_iterable(([](const int& i_value) { return 2 * i_value + 1; }));
const detail::__numbers_iterable even_integers_container = detail::__numbers_iterable(([](const int& i_value) { return 2 * i_value; }));

const detail::__numbers_iterable reals_container = detail::__numbers_iterable([](const float& i_value) { return i_value + 1.f; });
const detail::__numbers_iterable odd_reals_container = detail::__numbers_iterable(([](const float& i_value) { return 2 * i_value + 1; }));
const detail::__numbers_iterable even_reals_container = detail::__numbers_iterable(([](const float& i_value) { return 2 * i_value; }));

}

const const_forward_iterable<const int> integers = detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(integers_container));
const const_forward_iterable<const int> odd_integers = detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(odd_integers_container));
const const_forward_iterable<const int> even_integers = detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(even_integers_container));

const const_forward_iterable<const int> naturals = view::constrain([](const int& i_value){ return i_value > 0; }) <<= detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(integers_container));
const const_forward_iterable<const int> odd_naturals = view::constrain([](const int& i_value) { return i_value > 0; }) <<= detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(odd_integers_container));
const const_forward_iterable<const int> even_naturals = view::constrain([](const int& i_value) { return i_value > 0; }) <<= detail::owned_iterable<const int,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const int&>,begin_action_tag,forward_action_tag>>(deduce_iterable(even_integers_container));

const const_forward_iterable<const float> reals = detail::owned_iterable<const float,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const float&>,begin_action_tag,forward_action_tag>>(deduce_iterable(reals_container));
const const_forward_iterable<const float> odd_reals = detail::owned_iterable<const float,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const float&>,begin_action_tag,forward_action_tag>>(deduce_iterable(odd_reals_container));
const const_forward_iterable<const float> even_reals = detail::owned_iterable<const float,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<const float&>,begin_action_tag,forward_action_tag>>(deduce_iterable(even_reals_container));

}
