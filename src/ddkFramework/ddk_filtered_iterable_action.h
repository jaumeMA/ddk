#pragma once

#include "ddk_filtered_iterable_action_result.h"
#include "ddk_iterable_action_tags.h"
#include "ddk_iterable_action_tag_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{

template<typename ActionTag, typename Filter>
class filtered_iterable_action
{
public:
	typedef mpl::type_pack<ActionTag> tags_t;
	typedef ActionTag action_tag;

	filtered_iterable_action(const ActionTag& i_action,const Filter& i_filter);
	filtered_iterable_action(ActionTag&& i_action,const Filter& i_filter);
	filtered_iterable_action(const filtered_iterable_action& other) = default;
	filtered_iterable_action(filtered_iterable_action&&) = default;

	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	filtered_iterable_action_result<Adaptor,ActionTag,Filter> apply(Adaptor&& i_adaptor);
	filtered_iterable_action& operator=(const filtered_iterable_action&) = default;
	filtered_iterable_action& operator=(filtered_iterable_action&&) = default;

	const ActionTag& action() const &;
	ActionTag action() &&;
	const Filter& filter() const;

private:
	ActionTag m_actionTag;
	const Filter m_filter;
};
template<typename ActionTag, typename Filter>
filtered_iterable_action(const ActionTag&,const Filter&)->filtered_iterable_action<ActionTag,Filter>;
template<typename ActionTag,typename Filter>
filtered_iterable_action(ActionTag&&,const Filter&)->filtered_iterable_action<ActionTag,Filter>;

template<typename Filter>
class filtered_iterable_action<displace_action_tag,Filter>
{
public:
	typedef mpl::type_pack<displace_action_tag> tags_t;
	typedef displace_action_tag action_tag;

	filtered_iterable_action(const displace_action_tag& i_action,const Filter& i_filter);
	filtered_iterable_action(const filtered_iterable_action&) = default;

	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	filtered_iterable_action_result<Adaptor,displace_action_tag,Filter> apply(Adaptor&& i_adaptor);
	filtered_iterable_action& operator=(const filtered_iterable_action&) = default;

	const displace_action_tag& action() const;
	const Filter& filter() const;

private:
	displace_action_tag m_actionTag;
	const Filter m_filter;
};
template<typename Sink,typename Filter>
class filtered_iterable_action<sink_action_tag<Sink>,Filter>
{
public:
	typedef mpl::type_pack<sink_action_tag<Sink>> tags_t;
	typedef sink_action_tag<Sink> action_tag;

	filtered_iterable_action(const sink_action_tag<Sink>& i_action,const Filter& i_filter);
	filtered_iterable_action(sink_action_tag<Sink>&& i_action,const Filter& i_filter);
	filtered_iterable_action(const filtered_iterable_action&) = default;

	TEMPLATE(typename Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t))
	filtered_iterable_action_result<Adaptor,sink_action_tag<Sink>,Filter> apply(Adaptor&& i_adaptor);
	filtered_iterable_action& operator=(const filtered_iterable_action&) = default;

	const sink_action_tag<Sink>& action() const;
	const Filter& filter() const;

private:
	sink_action_tag<Sink> m_actionTag;
	const Filter m_filter;
};

}

#include "ddk_filtered_iterable_action.inl"