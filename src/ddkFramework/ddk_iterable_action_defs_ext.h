#pragma once

#include "ddk_variant.h"
#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename ... Actions>
struct any_action : public action_base
{
public:
	typedef typename mpl::merge_type_packs<typename Actions::tags_t...>::type tags_t;

	TEMPLATE(typename AAction)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(AAction,Actions...))
	constexpr any_action(AAction&& i_action, bool i_valid = true);

	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	constexpr inline auto apply(Adaptor&& i_adaptor) const;

private:
	TEMPLATE(typename AAction)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(AAction,Actions...))
	constexpr any_action(const variant<Actions...>& i_action, bool i_valid = true);

	variant<Actions...> m_actions;
};

struct swap_action : action_base
{
	TEMPLATE(typename Iterable)
	REQUIRES(IS_DEDUCIBLE_ITERABLE_TYPE(Iterable))
	friend inline auto operator>>=(const swap_action&,Iterable&& i_iterable)
	{
		typedef resolved_iterable_traits<Iterable> traits;
		typedef typename traits::value_type value_type;
		typedef typename traits::reference reference;

		const mpl::remove_qualifiers<value_type>* prevLeftValue = nullptr;
		const mpl::remove_qualifiers<value_type>* prevRightValue = nullptr;
		[&](reference i_lhs,reference i_rhs) mutable
		{
			const mpl::remove_qualifiers<value_type>* currLeftValue = &i_lhs;
			const mpl::remove_qualifiers<value_type>* currRightValue = &i_rhs;

			if (currLeftValue != currRightValue && prevLeftValue != currRightValue && prevRightValue != currLeftValue)
			{
				prevLeftValue = currLeftValue;
				prevRightValue = currRightValue;

				std::swap(i_lhs,i_rhs);
			}
			else
			{
				terminate_iteration();
			}
		} <<= ddk::fusion(std::forward<Iterable>(i_iterable),ddk::view::order(ddk::reverse_order) <<= std::forward<Iterable>(i_iterable));
	}

public:
	typedef mpl::type_pack<begin_action_tag,end_action_tag,forward_action_tag,backward_action_tag> tags_t;

	constexpr swap_action() = default;
	constexpr swap_action(bool i_valid);
};

}

#include "ddk_iterable_action_defs_ext.inl"