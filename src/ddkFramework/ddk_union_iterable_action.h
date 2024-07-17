//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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

	constexpr union_iterable_action() = default;
	constexpr union_iterable_action(begin_action_tag&&);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,begin_action_tag> apply(Adaptor&& i_adaptor);
	constexpr begin_action_tag action() const;
};

template<>
class union_iterable_action<begin_next_iterable>
{
public:
	typedef mpl::type_pack<begin_action_tag> tags_t;
	typedef begin_next_iterable action_tag;

	constexpr union_iterable_action() = default;
	constexpr union_iterable_action(begin_next_iterable&&);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,begin_next_iterable> apply(Adaptor&& i_adaptor);
	constexpr begin_next_iterable action() const;
};

template<>
class union_iterable_action<end_action_tag>
{
public:
	typedef mpl::type_pack<end_action_tag> tags_t;
	typedef end_action_tag action_tag;

	constexpr union_iterable_action() = default;
	constexpr union_iterable_action(end_action_tag&&);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,end_action_tag> apply(Adaptor&& i_adaptor);
	constexpr end_action_tag action() const;
};

template<>
class union_iterable_action<end_prev_iterable>
{
public:
	typedef mpl::type_pack<end_action_tag> tags_t;
	typedef end_prev_iterable action_tag;

	constexpr union_iterable_action() = default;
	constexpr union_iterable_action(end_prev_iterable&&);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,end_prev_iterable> apply(Adaptor&& i_adaptor);
	constexpr end_prev_iterable action() const;
};

template<>
class union_iterable_action<size_action_tag>
{
public:
	typedef mpl::type_pack<size_action_tag> tags_t;
	typedef size_action_tag action_tag;

	constexpr union_iterable_action() = default;
	constexpr union_iterable_action(const size_action_tag&);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,size_action_tag> apply(Adaptor&& i_adaptor);
	constexpr size_action_tag action() const;

private:
	template<typename Adaptor, size_t ... Indexs>
	constexpr inline union_iterable_action_result<Adaptor,size_action_tag> _apply(Adaptor&& i_adaptor, const mpl::sequence<Indexs...>&);
};

template<typename ActionTag>
class union_iterable_action
{
public:
	typedef mpl::type_pack<ActionTag> tags_t;
	typedef ActionTag action_tag;

	constexpr union_iterable_action() = default;
	TEMPLATE(typename AActionTag)
	REQUIRES(IS_CONSTRUCTIBLE(ActionTag,AActionTag))
	constexpr union_iterable_action(AActionTag&& i_actionTag);
	constexpr union_iterable_action(ActionTag&& i_actionTag);

	template<typename Adaptor>
	constexpr inline union_iterable_action_result<Adaptor,ActionTag> apply(Adaptor&& i_adaptor);
	constexpr const ActionTag& action() const;

private:
	ActionTag m_actionTag;
};
template<typename ActionTag>
union_iterable_action(const ActionTag&)->union_iterable_action<ActionTag>;
template<typename ActionTag>
union_iterable_action(ActionTag&&) -> union_iterable_action<ActionTag>;

}

#include "ddk_union_iterable_action.inl"