
#include "ddk_function_arguments_template_helper.h"
#include "ddk_iterable_action_tag_result.h"

namespace ddk
{

template<template<typename>typename Action,typename ... T>
constexpr intersection_action<Action<function_arguments<T...>>>::intersection_action(const Action<function_arguments<T...>>& i_action)
: m_action(i_action)
{
}
template<template<typename>typename Action,typename ... T>
constexpr intersection_action<Action<function_arguments<T...>>>::intersection_action(Action<function_arguments<T...>>&& i_action)
: m_action(std::move(i_action))
{
}
template<template<typename>typename Action,typename ... T>
TEMPLATE(size_t ... Indexs,typename ... Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t::template nth_type<Indexs>)...)
constexpr auto intersection_action<Action<function_arguments<T...>>>::apply(Adaptor&& ... i_adaptors)
{
	if ((std::forward<Adaptor>(i_adaptors).perform_action(Action<T>{ m_action.get().template get<Indexs>() }) && ...))
	{
		return make_result<iterable_action_tag_result<detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...>,Action<function_arguments<T...>>>>(success);
	}
	else
	{
		return make_error<iterable_action_tag_result<detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...>,Action<function_arguments<T...>>>>(std::move(m_action));
	}
}

template<typename Sink>
constexpr intersection_action<sink_action_tag<Sink>>::intersection_action(const sink_action_tag<Sink>& i_action)
: m_action(i_action)
{
}
template<typename Sink>
constexpr intersection_action<sink_action_tag<Sink>>::intersection_action(sink_action_tag<Sink>&& i_action)
: m_action(std::move(i_action))
{
}
template<typename Sink>
template<size_t ... Indexs,typename ... Adaptor>
constexpr auto intersection_action<sink_action_tag<Sink>>::apply(Adaptor&& ... i_adaptors)
{
	typedef tuple<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,k_iterable_empty_sink>...> adaptors_result;
	typedef detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...> intersection_traits;
	typedef typename intersection_traits::reference reference;
	typedef iterable_action_tag_result<intersection_traits,sink_action_tag<Sink>> intersection_result;

	if (auto actionRes = create_reference<reference>(std::forward<Adaptor>(i_adaptors).perform_action(std::forward<Adaptor>(i_adaptors),k_iterableEmptySink).dismiss() ...))
	{
		return make_result<intersection_result>(m_action(*actionRes));
	}
	else
	{
		return make_error<intersection_result>(std::move(m_action));
	}
}
template<typename Sink>
template<typename Reference, typename ... Result>
constexpr inline optional<Reference> intersection_action<sink_action_tag<Sink>>::create_reference(Result&& ... i_results)
{
	if ((static_cast<bool>(i_results) && ...))
	{
		return Reference{ i_results.get() ... };
	}
	else
	{
		return none;
	}
}

constexpr intersection_action<size_action_tag>::intersection_action(const size_action_tag&)
{
}
template<size_t ... Indexs,typename ... Adaptor>
constexpr auto intersection_action<size_action_tag>::apply(Adaptor&& ... i_adaptors)
{
	typedef tuple<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,size_action_tag>...> adaptors_result;
	typedef detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...> intersection_traits;
	typedef iterable_action_tag_result<intersection_traits,size_action_tag> intersection_result;

	adaptors_result actionRes = { std::forward<Adaptor>(i_adaptors).perform_action(std::forward<Adaptor>(i_adaptors),size_action_tag{}) ... };

	//in order to avoid unchecked result asserts, ensure we check all of them
	const bool actionResBool[mpl::num_ranks<Indexs...>] = { static_cast<bool>(actionRes.template get<Indexs>()) ... };

	if ((actionResBool[Indexs] && ...))
	{
		typedef typename intersection_traits::reference reference;

		return make_result<intersection_result>(mpl::get_min(actionRes.template get<Indexs>().get() ... ));
	}
	else
	{
		return make_error<intersection_result>(size_action_tag{});
	}
}

template<typename ActionTag>
constexpr intersection_action<ActionTag>::intersection_action(const ActionTag& i_action)
: m_action(i_action)
{
}
template<typename ActionTag>
constexpr intersection_action<ActionTag>::intersection_action(ActionTag&& i_action)
: m_action(std::move(i_action))
{
}
template<typename ActionTag>
TEMPLATE(size_t ... Indexs,typename ... Adaptor)
REQUIRED(ACTION_TAGS_SUPPORTED(Adaptor,tags_t)...)
constexpr auto intersection_action<ActionTag>::apply(Adaptor&& ... i_adaptors)
{
	if constexpr ((IS_ADAPTOR_REPRESENTABLE_BY_ACTION_COND(Adaptor,ActionTag) && ...))
	{
		typedef detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...> intersection_traits;
		typedef typename intersection_traits::reference reference;
		typedef tuple<iterable_action_tag_result<detail::adaptor_traits<Adaptor>,ActionTag>...> adaptors_result;
		typedef iterable_action_tag_result<intersection_traits,ActionTag> intersection_result;

		if(auto actionRes = create_reference<reference>(std::forward<Adaptor>(i_adaptors).perform_action(std::forward<Adaptor>(i_adaptors),std::move(m_action)).dismiss() ...))
		{
			return make_result<intersection_result>(*actionRes);
		}
		else
		{
			return make_error<intersection_result>(std::move(m_action));
		}
	}
	else
	{
		typedef iterable_action_tag_result<detail::intersection_iterable_traits<detail::adaptor_traits<Adaptor>...>,ActionTag> intersection_result;

		if ((std::forward<Adaptor>(i_adaptors).perform_action(std::forward<Adaptor>(i_adaptors),std::move(m_action)) && ...))
		{
			return make_result<intersection_result>(success);
		}
		else
		{
			return make_error<intersection_result>(std::move(m_action));
		}
	}
}
template<typename ActionTag>
template<typename Reference,typename ... Result>
constexpr inline optional<Reference> intersection_action<ActionTag>::create_reference(Result&& ... i_results)
{
	if ((static_cast<bool>(i_results) && ...))
	{
		return Reference{ i_results.get() ... };
	}
	else
	{
		return none;
	}
}

}