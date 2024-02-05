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
	any_action(AAction&& i_action, bool i_valid = true);

	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	inline auto apply(Adaptor&& i_adaptor) const;

private:
	TEMPLATE(typename AAction)
	REQUIRES(IS_AMONG_CONSTRUCTIBLE_TYPES(AAction,Actions...))
	any_action(const variant<Actions...>& i_action, bool i_valid = true);

	variant<Actions...> m_actions;
};

}

#include "ddk_iterable_action_defs_ext.inl"