#pragma once

#include "ddk_iterable_action.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<bool Forward>
class reversable_order_resolver
{
public:
	reversable_order_resolver() = default;

	TEMPLATE(typename Action)
	REQUIRES(ANY_ACTION_TAGS_SUPPORTED(Action,forward_action_tag,backward_action_tag,displace_action_tag))
	inline auto operator()(const Action& i_action) const;
};
typedef reversable_order_resolver<true> forward_order_resolver;
typedef reversable_order_resolver<false> backward_order_resolver;

class transpose_multi_dimensional_order_resolver
{
public:
	struct action : action_base
	{
		friend inline action inverse(const action& other)
		{
			return { -other.m_shift };
		}

		typedef mpl::type_pack<displace_action_tag> tags_t;
		typedef long long difference_type;

		action(difference_type i_targetShift);
		action(const stop_action&);

		difference_type shift() const;
		TEMPLATE(typename Adaptor,typename Sink)
		REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
		inline auto apply(Adaptor&& i_adaptor,Sink&& i_sink) const;

	private:
		mutable difference_type m_shift = 0;
	};

	transpose_multi_dimensional_order_resolver() = default;

	TEMPLATE(typename Action)
	REQUIRES(ANY_ACTION_TAGS_SUPPORTED(Action,begin_action_tag,forward_action_tag,backward_action_tag,displace_action_tag))
	action operator()(const Action& i_action) const;
};

}

extern const detail::forward_order_resolver forward_order;
extern const detail::backward_order_resolver reverse_order;
extern const detail::transpose_multi_dimensional_order_resolver transponse_dimension_order;

}