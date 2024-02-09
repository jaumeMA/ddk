#pragma once

#include "ddk_iterable_visitor.h"
#include "ddk_iterable_action_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T>
class iterable_order
{
public:
	iterable_order(const T& i_order);

	const T& order() const;

private:
	const T m_order;
};

template<typename Iterable,typename ActionResolver>
class ordered_iterable_impl : public iterable_impl_interface<typename Iterable::traits>, public iterable_visitor<ordered_iterable_impl<Iterable,ActionResolver>>
{
public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;

	TEMPLATE(typename IIterable,typename AActionResolver)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(ActionResolver,AActionResolver))
	ordered_iterable_impl(IIterable&& i_iterable,AActionResolver&& i_actionResolver);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	iterable_result iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	iterable_result iterate_impl(Action&& i_initialAction) const;
};

}

template<typename Iterable,typename ActionResolver>
class iterable_adaptor<detail::ordered_iterable_impl<Iterable,ActionResolver>>
{
public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterable& i_iterable,const ActionResolver& i_actionResolver);
	iterable_adaptor(detail::ordered_iterable_impl<Iterable,ActionResolver>& i_iterable);

	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag) const;

private:
	deduced_adaptor<Iterable> m_adaptor;
	const ActionResolver m_actionResolver;
};

template<typename Iterable,typename ActionResolver>
class iterable_adaptor<const detail::ordered_iterable_impl<Iterable,ActionResolver>>
{
public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterable& i_iterable,const ActionResolver& i_actionResolver);

	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag) const;

private:
	deduced_adaptor<const Iterable> m_adaptor;
	const ActionResolver m_actionResolver;
};

}

#include "ddk_ordered_iterable_impl.inl"