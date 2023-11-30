#pragma once

#include "ddk_result_success.h"
#include "ddk_optional.h"
#include "ddk_type_concepts.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename,bool>
struct iterable_action_recovery;
	
template<typename ActionTag>
struct iterable_action_recovery<ActionTag,true>
{
public:
	typedef typename ActionTag::recovery_tag recovery_tag;

	iterable_action_recovery(const result_success_t&);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(recovery_tag,Args...))
	iterable_action_recovery(Args&& ... i_args);

	operator bool() const;
	bool recoverable() const;
	recovery_tag recovery() const;

private:
	optional<recovery_tag> m_recovery;
};

template<typename ActionTag>
struct iterable_action_recovery<ActionTag,false>
{
public:
	iterable_action_recovery() = default;
	iterable_action_recovery(const result_success_t&);

	operator bool() const;
	bool recoverable() const;
	ActionTag recovery() const;

private:
	bool m_success = false;
};

template<typename ActionTag>
struct iterable_action_result_impl : protected iterable_action_recovery<ActionTag,IS_ACTION_TAG_RECOVERABLE_COND(ActionTag)>
{
	friend inline iterable_action_result_impl operator&&(const iterable_action_result_impl& i_lhs,const iterable_action_result_impl& i_rhs)
	{
		if (static_cast<bool>(i_lhs) && static_cast<bool>(i_rhs))
		{
			return success;
		}
		else if (static_cast<bool>(i_lhs))
		{
			return i_rhs;
		}
		else if (static_cast<bool>(i_rhs))
		{
			return i_lhs;
		}
		else
		{
			return i_lhs;//temporary
		}
	}

	typedef iterable_action_recovery<ActionTag,IS_ACTION_TAG_RECOVERABLE_COND(ActionTag)> base_t;

public:
	using base_t::operator bool;
	using base_t::recoverable;
	using base_t::recovery;

	iterable_action_result_impl(const result_success_t&);
	template<typename ... Args>
	iterable_action_result_impl(Args&& ... i_args);
};


}

template<typename ActionTag>
using iterable_action_result = detail::iterable_action_result_impl<mpl::remove_qualifiers<ActionTag>>;

}

#include "ddk_iterable_action_result.inl"