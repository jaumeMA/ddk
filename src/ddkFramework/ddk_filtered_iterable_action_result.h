#pragma once

#include "ddk_filtered_iterable_action_recovery_tags.h"
#include "ddk_iterable_action_tag_result.h"
#include "ddk_optional.h"

namespace ddk
{

template<typename Traits,typename ActionTag,typename Filter>
struct filtered_iterable_action_error
{
	typedef iterable_action_tag_error<Traits,filtered_iterable_action<ActionTag,Filter>> iterable_action_error;

public:
	typedef typename iterable_action_error::recovery_tag recovery_tag;

	constexpr filtered_iterable_action_error() = default;
	constexpr filtered_iterable_action_error(const Filter& i_filter);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(iterable_action_error,Args...))
	constexpr filtered_iterable_action_error(Args&& ... i_args);
	constexpr filtered_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error,const Filter& i_filter, bool i_filteredOut);
	constexpr filtered_iterable_action_error(iterable_action_tag_error<Traits,ActionTag>&& i_error,const Filter& i_filter,bool i_filteredOut);
	template<typename AActionTag>
	constexpr filtered_iterable_action_error(const filtered_iterable_action_error<Traits,AActionTag,Filter>& i_error);
	template<typename AActionTag>
	constexpr filtered_iterable_action_error(filtered_iterable_action_error<Traits,AActionTag,Filter>&& i_error);

	constexpr operator bool() const;
	TEMPLATE(typename TTraits)
	REQUIRES(ADAPTOR_TAGS_EQUIVALENT(Traits,TTraits))
	constexpr operator iterable_action_tag_error<TTraits,ActionTag>() const &;
	template<typename TTraits>
	constexpr operator iterable_action_tag_error<TTraits,ActionTag>() &&;
	constexpr auto recovery() const&;
	constexpr auto recovery()&&;

private:
	optional<iterable_action_error> m_error;
	bool m_filteredOut = true;
};
template<typename Adaptor,typename ActionTag,typename Filter>
using filtered_iterable_action_result = result<iterable_action_return_type<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>>,filtered_iterable_action_error<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>,Filter>>;

}

#include "ddk_filtered_iterable_action_conversion_sequence.h"
#include "ddk_filtered_iterable_action_result.inl"