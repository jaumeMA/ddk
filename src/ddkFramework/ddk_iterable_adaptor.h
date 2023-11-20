#pragma once

#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename>
class iterable_adaptor_base;

template<typename Iterable>
class iterable_adaptor_base
{
public:
	typedef typename Iterable::value_type value_type;
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_reference,typename Iterable::reference>::type reference;
	typedef typename Iterable::const_reference const_reference;
	typedef long long difference_type;
	typedef typename mpl::which_type<std::is_const<Iterable>::value,mpl::type_pack<begin_action_tag,last_action_tag>,mpl::type_pack<begin_action_tag,last_action_tag,add_action_tag<value_type>,remove_action_tag>>::type tags_t;

	iterable_adaptor_base(Iterable& i_iterable);

	inline reference get_value();
	inline const_reference get_value() const;
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink);
	template<typename Sink>
	inline auto forward_value(Sink&& i_sink) const;
	inline bool valid() const noexcept;
	inline bool perform_action(const begin_action_tag&) const;
	inline bool perform_action(const last_action_tag&) const;
	inline bool perform_action(const remove_action_tag&);
	inline bool perform_action(add_action_tag<value_type>);

protected:
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_iterator,typename Iterable::iterator>::type iterator;
	typedef typename Iterable::const_iterator const_iterator;

	Iterable& m_iterable;
	mutable iterator m_currIterator;
	const iterator m_endIterator;
};

template<typename Iterable>
class forward_iterable_adaptor : public iterable_adaptor_base<Iterable>
{
	typedef iterable_adaptor_base<Iterable> base_t;
public:
	using base_t::base_t;
	using base_t::perform_action;
	using typename base_t::value_type;
	using typename base_t::reference;
	using typename base_t::const_reference;
	using typename base_t::difference_type;
	typedef typename base_t::tags_t::template add_unique<forward_action_tag>::type tags_t;

	inline bool perform_action(const forward_action_tag&) const;
};

template<typename Iterable>
class bidirectional_iterable_adaptor : public forward_iterable_adaptor<Iterable>
{
	typedef forward_iterable_adaptor<Iterable> base_t;

public:
	using base_t::perform_action;
	using typename base_t::value_type;
	using typename base_t::reference;
	using typename base_t::const_reference;
	using typename base_t::difference_type;
	typedef typename base_t::tags_t::template add_unique<backward_action_tag>::type tags_t;

	bidirectional_iterable_adaptor(Iterable& i_iterable);

	inline bool perform_action(const backward_action_tag&) const;

protected:
	const iterator m_beginIterator;
};

template<typename>
class random_access_iterable_adaptor;

template<typename Iterable>
class random_access_iterable_adaptor : public bidirectional_iterable_adaptor<Iterable>
{
	typedef bidirectional_iterable_adaptor<Iterable> base_t;

public:
	using base_t::base_t;
	using base_t::perform_action;
	using typename base_t::value_type;
	using typename base_t::reference;
	using typename base_t::const_reference;
	using typename base_t::difference_type;
	typedef typename base_t::tags_t::template add_unique<displace_action_tag>::type tags_t;

	inline bool perform_action(const displace_action_tag&) const;
};

}
}

#include "ddk_iterable_adaptor.inl"