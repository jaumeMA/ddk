#pragma once

#include "ddk_iterable_action_tags.h"
#include "ddk_function_template_helper.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_optional.h"

namespace ddk
{

template<typename>
class intersection_action;

template<template<typename>typename Action,typename ... T>
class intersection_action<Action<function_arguments<T...>>>
{
public:
	typedef mpl::type_pack<Action<T>...> tags_t;

	constexpr intersection_action(const Action<function_arguments<T...>>& i_action);
	constexpr intersection_action(Action<function_arguments<T...>>&& i_action);
	TEMPLATE(size_t ... Indexs,typename ... Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t::template nth_type<Indexs>)...)
	constexpr inline auto apply(Adaptor&& ... i_adaptors);

private:
	Action<function_arguments<T...>> m_action;
};

template<typename Sink>
class intersection_action<sink_action_tag<Sink>>
{
	typedef typename mpl::aqcuire_callable_args_type<Sink>::type args_t;
	typedef typename mpl::make_transformed_tuple<optional,args_t>::type opt_args_t;

public:
	typedef mpl::type_pack<sink_action_tag<Sink>> tags_t;

	constexpr intersection_action(const sink_action_tag<Sink>& i_action);
	constexpr intersection_action(sink_action_tag<Sink>&& i_action);
	template<size_t ... Indexs, typename ... Adaptor>
	constexpr inline auto apply(Adaptor&& ... i_adaptors);

private:
	template<typename Reference, typename ... Result>
	static constexpr inline optional<Reference> create_reference(Result&& ... i_result);

	sink_action_tag<Sink> m_action;
	opt_args_t m_args;
};
template<typename Sink>
intersection_action(const sink_action_tag<Sink>&) -> intersection_action<sink_action_tag<Sink>>;
template<typename Sink>
intersection_action(sink_action_tag<Sink>&&) -> intersection_action<sink_action_tag<Sink>>;

template<>
class intersection_action<size_action_tag>
{
public:
	typedef mpl::type_pack<size_action_tag> tags_t;

	constexpr intersection_action(const size_action_tag&);
	template<size_t ... Indexs,typename ... Adaptor>
	constexpr inline auto apply(Adaptor&& ... i_adaptors);
};

template<typename ActionTag>
class intersection_action
{
public:
	typedef mpl::type_pack<ActionTag> tags_t;

	constexpr intersection_action(const ActionTag& i_action);
	constexpr intersection_action(ActionTag&& i_action);
	TEMPLATE(size_t ... Indexs,typename ... Adaptor)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,tags_t)...)
	constexpr inline auto apply(Adaptor&& ... i_adaptors);

private:
	ActionTag m_action;
};
template<typename T>
intersection_action(const T&) -> intersection_action<T>;
template<typename T>
intersection_action(T&&) -> intersection_action<T>;

}

#include "ddk_intersection_iterable_action.inl"