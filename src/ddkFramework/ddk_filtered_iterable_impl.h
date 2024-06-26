//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"
#include "ddk_filtered_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_filter
{
public:
	iterable_filter(const Function& i_filter);

	Function get_filter() const;

private:
	const Function m_filter;
};

template<typename Iterable, typename Filter>
class filtered_iterable_impl : public iterable_impl_interface<typename Iterable::traits>, public iterable_visitor<filtered_iterable_impl<Iterable,Filter>>
{
	typedef iterable_visitor<filtered_iterable_impl<Iterable,Filter>> base_t;

public:
    typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;

    TEMPLATE(typename IIterable,typename FFilter)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Filter,FFilter))
    filtered_iterable_impl(IIterable&& i_iterable, FFilter&& i_filter);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	void iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	void iterate_impl(Action&& i_initialAction) const;
};

}

template<typename Iterable,typename Filter>
class iterable_adaptor<detail::filtered_iterable_impl<Iterable,Filter>>
{
public:
	typedef Filter filter;
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterable& i_iterable,const Filter& i_filter);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static constexpr auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	template<typename Adaptor, typename ActionTag>
	static constexpr auto perform_action(Adaptor&& i_adaptor, filtered_iterable_action<ActionTag,Filter> i_actionTag);

	deduced_adaptor<Iterable> m_adaptor;
	const filter m_filter;
};

template<typename Iterable,typename Filter>
class iterable_adaptor<const detail::filtered_iterable_impl<Iterable,Filter>>
{
public:
	typedef Filter filter;
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterable& i_iterable,const Filter& i_actionResolver);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static constexpr auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	template<typename Adaptor, typename ActionTag>
	static constexpr auto perform_action(Adaptor&& i_adaptor, filtered_iterable_action<ActionTag,Filter> i_actionTag);

	deduced_adaptor<const Iterable> m_adaptor;
	const filter m_filter;
};

}

#include "ddk_filtered_iterable_impl.inl"