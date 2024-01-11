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

	filtered_iterable_action_error() = default;
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(iterable_action_error,Args...))
	filtered_iterable_action_error(Args&& ... i_args);
	filtered_iterable_action_error(const iterable_action_tag_error<Traits,ActionTag>& i_error,const Filter& i_filter, bool i_filteredOut);
	template<typename AActionTag>
	filtered_iterable_action_error(const filtered_iterable_action_error<Traits,AActionTag,Filter>& i_error);

	operator bool() const;
	operator iterable_action_tag_error<Traits,ActionTag>() const;
	auto recovery() const&;
	auto recovery()&&;

private:
	optional<iterable_action_error> m_error;
	bool m_filteredOut = true;
};
template<typename Adaptor,typename ActionTag,typename Filter>
using filtered_iterable_action_result = result<iterable_action_return_type<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>>,filtered_iterable_action_error<detail::adaptor_traits<Adaptor>,mpl::remove_qualifiers<ActionTag>,Filter>>;

}

#include "ddk_filtered_iterable_action_conversion_sequence.h"
#include "ddk_filtered_iterable_action_result.inl"