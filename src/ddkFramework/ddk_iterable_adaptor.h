#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_action.h"
#include "ddk_iterable_action_tag_result.h"

namespace ddk
{

template<typename>
struct iterable_adaptor;

namespace detail
{

template<typename>
class iterable_adaptor_base;

template<typename Iterable>
class iterable_adaptor_base
{
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename std::add_const<typename Iterable::value_type>::type,typename Iterable::value_type>::type value_type;
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_reference,typename Iterable::reference>::type reference;
	typedef typename Iterable::const_reference const_reference;

public:
	typedef iterable_by_type_traits<value_type,
									reference,
									const_reference,
									typename mpl::which_type<mpl::is_const<Iterable>,mpl::type_pack<sink_action_tag<function<void(reference)>>>,mpl::type_pack<sink_action_tag<function<void(reference)>>,add_action_tag<typename Iterable::value_type>,remove_action_tag>>::type,
									mpl::type_pack<sink_action_tag<function<void(const_reference)>>,begin_action_tag,last_action_tag>> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor_base(Iterable& i_iterable);

	template<typename Sink>
	inline auto perform_action(const sink_action_tag<Sink>& i_sink);
	template<typename Sink>
	inline auto perform_action(const sink_action_tag<Sink>& i_sink) const;
	inline auto perform_action(const begin_action_tag&) const;
	inline auto perform_action(const last_action_tag&) const;
	inline auto perform_action(const remove_action_tag&);
	inline auto perform_action(add_action_tag<value_type>);

protected:
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_iterator,typename Iterable::iterator>::type iterator;

	Iterable& m_iterable;
	mutable iterator m_currIterator;
	const iterator m_endIterator;
};

template<typename Iterable>
class forward_iterable_adaptor : public iterable_adaptor_base<Iterable>
{
	typedef iterable_adaptor_base<Iterable> base_t;
	typedef typename base_t::traits base_traits;

public:
	typedef iterable_adaptor_traits<base_traits,typename base_traits::tags_t,typename base_traits::const_tags_t::template add_unique<forward_action_tag>::type> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	using base_t::base_t;
	using base_t::perform_action;

	inline auto perform_action(const forward_action_tag&) const;
};

template<typename Iterable>
class bidirectional_iterable_adaptor : public forward_iterable_adaptor<Iterable>
{
	typedef forward_iterable_adaptor<Iterable> base_t;
	typedef typename base_t::traits base_traits;

public:
	typedef iterable_adaptor_traits<base_traits,typename base_traits::tags_t,typename base_traits::const_tags_t::template add_unique<backward_action_tag>::type> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	using base_t::perform_action;

	bidirectional_iterable_adaptor(Iterable& i_iterable);

	inline auto perform_action(const backward_action_tag&) const;

protected:
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_iterator,typename Iterable::iterator>::type iterator;

	const iterator m_beginIterator;
};

template<typename>
class random_access_iterable_adaptor;

template<typename Iterable>
class random_access_iterable_adaptor : public bidirectional_iterable_adaptor<Iterable>
{
	typedef bidirectional_iterable_adaptor<Iterable> base_t;
	typedef typename base_t::traits base_traits;

public:
	typedef iterable_adaptor_traits<base_traits,typename base_traits::tags_t,typename base_traits::const_tags_t::template add_unique<displace_action_tag>::type> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	using base_t::base_t;
	using base_t::perform_action;

	inline auto perform_action(const displace_action_tag&) const;
};

}
}

#include "ddk_iterable_adaptor.inl"