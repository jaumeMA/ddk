#pragma once

#include "ddk_result.h"
#include "ddk_iterable_traits_resolver.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename Traits,typename ActionTag>
struct iterable_action_tag_error
{
public:
	typedef iterable_action_recovery_tag<Traits,ActionTag> recovery_tag;

	constexpr iterable_action_tag_error() = default;
	TEMPLATE(typename TTraits, typename AActionTag)
	REQUIRES(IS_CONSTRUCTIBLE(recovery_tag,iterable_action_recovery_tag<TTraits,AActionTag>))
	constexpr iterable_action_tag_error(const iterable_action_tag_error<TTraits,AActionTag>& other);
	TEMPLATE(typename TTraits, typename AActionTag)
	REQUIRES(IS_CONSTRUCTIBLE(recovery_tag,iterable_action_recovery_tag<TTraits,AActionTag>))
	constexpr iterable_action_tag_error(iterable_action_tag_error<TTraits,AActionTag>&& other);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(recovery_tag,Args...))
	constexpr iterable_action_tag_error(Args&& ... i_args);

	constexpr inline const recovery_tag& recovery() const&;
	constexpr inline recovery_tag recovery()&&;
	template<typename Result>
	constexpr inline Result forward() const;

private:
	recovery_tag m_recovery;
};

template<typename Traits, typename ActionTag>
using iterable_action_tag_result = result<iterable_action_return_type<Traits,ActionTag>,iterable_action_tag_error<Traits,mpl::remove_qualifiers<ActionTag>>>;

}

#include "ddk_iterable_action_tag_result.inl"
#include "ddk_iterable_action_tag_result_ops.h"