#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_constrain
{
public:
	iterable_constrain(const Function& i_transform);

	Function get_constrain() const;

private:
	const Function m_constrain;
};
template<typename Function>
iterable_constrain(Function&&) -> iterable_constrain<mpl::remove_qualifiers<Function>>;

template<typename Iterable,typename Constrain>
class constrained_iterable_impl : public iterable_impl_interface<typename Iterable::traits>, public iterable_visitor<constrained_iterable_impl<Iterable,Constrain>>
{
	typedef iterable_visitor<constrained_iterable_impl<Iterable,Constrain>> base_t;

public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;

	TEMPLATE(typename IIterable,typename CConstrain)
	REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Constrain,CConstrain))
	constrained_iterable_impl(IIterable&& i_iterable,CConstrain&& i_constrain);

	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(traits,Action))
	void iterate_impl(Action&& i_initialAction);
	TEMPLATE(typename Action)
	REQUIRES(ACTION_SUPPORTED(const_traits,Action))
	void iterate_impl(Action&& i_initialAction) const;
};

}

template<typename Iterable,typename Constrain>
class iterable_adaptor<detail::constrained_iterable_impl<Iterable,Constrain>>
{
public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterable& i_iterable,const Constrain& i_constrain);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	deduced_adaptor<Iterable> m_adaptor;
	const Constrain m_constrain;
};

template<typename Iterable,typename Constrain>
class iterable_adaptor<const detail::constrained_iterable_impl<Iterable,Constrain>>
{
public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterable& i_iterable,const Constrain& i_constrain);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	deduced_adaptor<const Iterable> m_adaptor;
	const Constrain m_constrain;
};

}

#include "ddk_constrained_iterable_impl.inl"