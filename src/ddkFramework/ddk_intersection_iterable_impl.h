//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

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

	TEMPLATE(typename ... IIterables)
	REQUIRES(IS_CONSTRUCTIBLE(Iterables,IIterables)...)
    intersection_iterable_impl(IIterables&& ... i_iterables);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	void iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	void iterate_impl(Action&& i_initialAction) const;
};
template<typename ... Iterables>
intersection_iterable_impl(Iterables&&...) -> intersection_iterable_impl<mpl::remove_qualifiers<Iterables>...>;
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
	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	template<size_t ... Indexs,typename Adaptor,typename ActionTag>
	static auto perform_action(const mpl::sequence<Indexs...>&, Adaptor&& i_adaptor, intersection_action<ActionTag> i_actionTag);

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
	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static constexpr auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	template<size_t ... Indexs,typename Adaptor,typename ActionTag>
	static constexpr auto perform_action(const mpl::sequence<Indexs...>&,Adaptor&& i_adaptor,intersection_action<ActionTag> i_actionTag);

	const tuple<deduced_adaptor<Iterables>...> m_adaptors;
};

}

#include "ddk_intersection_iterable_impl.inl"