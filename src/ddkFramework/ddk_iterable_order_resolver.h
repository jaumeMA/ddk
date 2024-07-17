//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_action.h"
#include "ddk_iterable_adaptor_concepts.h"

namespace ddk
{
namespace detail
{

class forward_order_resolver
{
public:
	forward_order_resolver() = default;

	TEMPLATE(typename Adaptor,typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> operator()(Adaptor&& i_adaptor, ActionTag&& i_action) const;
};

class backward_order_resolver
{
public:
	constexpr backward_order_resolver() = default;

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> operator()(Adaptor&& i_adaptor, ActionTag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,end_action_tag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,begin_action_tag> operator()(Adaptor&& i_adaptor,begin_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,begin_action_tag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,end_action_tag> operator()(Adaptor&& i_adaptor,end_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag> operator()(Adaptor&& i_adaptor, forward_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag> operator()(Adaptor&& i_adaptor, backward_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,displace_action_tag))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag> operator()(Adaptor&& i_adaptor, displace_action_tag&& i_action) const;
};

class transpose_multi_dimensional_order_resolver
{
public:
	constexpr transpose_multi_dimensional_order_resolver() = default;

	TEMPLATE(typename Adaptor,typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag> operator()(Adaptor&& i_adaptor, ActionTag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,forward_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,forward_action_tag> operator()(Adaptor&& i_adaptor,forward_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,backward_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,backward_action_tag> operator()(Adaptor&& i_adaptor,backward_action_tag&& i_action) const;
	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,displace_action_tag),IS_DIMENSIONABLE_ADAPTOR(Adaptor))
	constexpr inline iterable_action_tag_result<detail::adaptor_traits<Adaptor>,displace_action_tag> operator()(Adaptor&& i_adaptor,displace_action_tag&& i_action) const;

private:
	template<typename Adaptor>
	constexpr inline auto perform_action(Adaptor&& i_adaptor, typename mpl::remove_qualifiers<Adaptor>::traits::difference_type i_displacement) const;
};

}

extern const detail::forward_order_resolver forward_order;
extern const detail::backward_order_resolver reverse_order;
extern const detail::transpose_multi_dimensional_order_resolver transponse_dimension_order;

}