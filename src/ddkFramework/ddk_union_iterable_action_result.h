#pragma once

#include "ddk_union_iterable_action_recovery_tags.h"

namespace ddk
{

template<typename Traits,typename ActionTag>
struct union_iterable_action_error : public iterable_action_tag_error<Traits,union_iterable_action<ActionTag>>
{
	typedef iterable_action_tag_error<Traits,union_iterable_action<ActionTag>> base_t;

public:
	using base_t::base_t;
	using base_t::recovery;
	using typename base_t::recovery_tag;

	union_iterable_action_error() = default;
	union_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error);
	union_iterable_action_error(iterable_action_tag_error<Traits,ActionTag>&& i_error);
	template<typename TTraits, typename AActionTag>
	union_iterable_action_error(const iterable_action_tag_error<TTraits,AActionTag>& i_error);
	template<typename TTraits,typename AActionTag>
	union_iterable_action_error(iterable_action_tag_error<TTraits,AActionTag>&& i_error);
	template<typename AActionTag>
	union_iterable_action_error(const union_iterable_action_error<Traits,AActionTag>& i_error);
	template<typename AActionTag>
	union_iterable_action_error(union_iterable_action_error<Traits,AActionTag>&& i_error);

	template<typename AActionTag>
	operator iterable_action_tag_error<Traits,AActionTag>() const &;
	template<typename AActionTag>
	operator iterable_action_tag_error<Traits,AActionTag>() &&;
};
template<typename Adaptor,typename ActionTag>
using union_iterable_action_result = result<iterable_action_return_type<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>>,union_iterable_action_error<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>>>;

}

#include "ddk_union_iterable_action_result.inl"