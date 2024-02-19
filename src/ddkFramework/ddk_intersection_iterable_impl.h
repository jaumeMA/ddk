#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_traits.h"
#include "ddk_intersection_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename ... Iterables>
class intersection_iterable_impl : public iterable_impl_interface<intersection_iterable_traits<typename Iterables::traits ...>>, public iterable_visitor<intersection_iterable_impl<Iterables...>>
{
    static const size_t s_numTypes = tuple<Iterables...>::size();

public:
	typedef intersection_iterable_traits<typename Iterables::traits ...> traits;
	typedef const_iterable_traits<traits> const_traits;

    intersection_iterable_impl(Iterables& ... i_iterables);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	void iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	void iterate_impl(Action&& i_initialAction) const;
};
template<typename ... Iterables>
intersection_iterable_impl(Iterables&...) -> intersection_iterable_impl<Iterables...>;

}

template<typename ... Iterables>
class iterable_adaptor<detail::intersection_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();
	static_assert(s_numTypes != 0,"You shall provide any iterable");

public:
	typedef detail::intersection_iterable_traits<typename Iterables::traits...> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterables& ... i_iterable);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	auto perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	auto perform_action(ActionTag&& i_actionTag) const;

private:
	template<size_t ... Indexs,typename ActionTag>
	auto perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag);
	template<size_t ... Indexs,typename ActionTag>
	auto perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag) const;

	tuple<deduced_adaptor<Iterables>...> m_adaptors;
};

template<typename ... Iterables>
class iterable_adaptor<const detail::intersection_iterable_impl<Iterables...>>
{
	static const size_t s_numTypes = mpl::get_num_types<Iterables...>();
	static_assert(s_numTypes != 0,"You shall provide any iterable");

public:
	typedef detail::intersection_iterable_traits<typename Iterables::traits...> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterables& ... i_iterable);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	auto perform_action(ActionTag&& i_actionTag) const;

private:
	template<size_t ... Indexs,typename ActionTag>
	auto perform_action(const mpl::sequence<Indexs...>&,intersection_action<ActionTag> i_actionTag) const;

	const tuple<deduced_adaptor<Iterables>...> m_adaptors;
};

}

#include "ddk_intersection_iterable_impl.inl"