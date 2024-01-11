#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_union_iterable_action_result.h"

namespace ddk
{

template<typename>
class union_iterable_action;

template<>
class union_iterable_action<begin_action_tag>
{
public:
	typedef mpl::type_pack<begin_action_tag> tags_t;
	typedef begin_action_tag action_tag;

	union_iterable_action(begin_action_tag&&);

	template<typename Adaptor>
	inline union_iterable_action_result<Adaptor,begin_action_tag> apply(Adaptor&& i_adaptor);
	begin_action_tag action() const;
};

template<>
class union_iterable_action<begin_next_iterable>
{
public:
	typedef mpl::type_pack<begin_action_tag> tags_t;
	typedef begin_next_iterable action_tag;

	union_iterable_action() = default;
	union_iterable_action(begin_next_iterable&&);

	template<typename Adaptor>
	inline union_iterable_action_result<Adaptor,begin_next_iterable> apply(Adaptor&& i_adaptor);
	begin_next_iterable action() const;
};

template<>
class union_iterable_action<last_action_tag>
{
public:
	typedef mpl::type_pack<last_action_tag> tags_t;
	typedef last_action_tag action_tag;

	union_iterable_action() = default;
	union_iterable_action(last_action_tag&&);

	template<typename Adaptor>
	inline union_iterable_action_result<Adaptor,last_action_tag> apply(Adaptor&& i_adaptor);
	last_action_tag action() const;
};

template<>
class union_iterable_action<last_prev_iterable>
{
public:
	typedef mpl::type_pack<last_action_tag> tags_t;
	typedef last_prev_iterable action_tag;

	union_iterable_action() = default;
	union_iterable_action(last_prev_iterable&&);

	template<typename Adaptor>
	inline union_iterable_action_result<Adaptor,last_prev_iterable> apply(Adaptor&& i_adaptor);
	last_prev_iterable action() const;
};

template<typename ActionTag>
class union_iterable_action
{
public:
	typedef mpl::type_pack<ActionTag> tags_t;
	typedef ActionTag action_tag;

	union_iterable_action() = default;
	TEMPLATE(typename AActionTag)
	REQUIRES(IS_CONSTRUCTIBLE(ActionTag,AActionTag))
	union_iterable_action(AActionTag&& i_actionTag);
	union_iterable_action(ActionTag&& i_actionTag);

	template<typename Adaptor>
	inline union_iterable_action_result<Adaptor,ActionTag> apply(Adaptor&& i_adaptor);
	const ActionTag& action() const;

private:
	ActionTag m_actionTag;
};
template<typename ActionTag>
union_iterable_action(const ActionTag&)->union_iterable_action<ActionTag>;
template<typename ActionTag>
union_iterable_action(ActionTag&&) -> union_iterable_action<ActionTag>;

}

#include "ddk_union_iterable_action.inl"