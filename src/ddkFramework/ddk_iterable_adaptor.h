#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_action.h"
#include "ddk_iterable_action_tag_result.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
class iterable_adaptor_actions;

template<typename>
struct adaptor_iterable_impl;

template<typename Iterable, typename Actions>
struct adaptor_iterable_impl<iterable_adaptor_actions<Iterable,Actions>>
{
	typedef Iterable type;
};

template<typename Adaptor>
using adaptor_iterable = typename adaptor_iterable_impl<Adaptor>::type;
template<typename Iterable>
using iterable_value_type = typename mpl::which_type<std::is_const<Iterable>::value,typename std::add_const<typename Iterable::value_type>::type,typename Iterable::value_type>::type;
template<typename Iterable>
using iterable_reference = typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_reference,typename Iterable::reference>::type;
template<typename Iterable>
using iterable_const_reference = typename Iterable::const_reference;
template<typename Iterable>
using iterable_iterator = typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_iterator,typename Iterable::iterator>::type;

template<typename,typename>
class iterable_adaptor_action;

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,begin_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<begin_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable);

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const begin_action_tag&);

protected:
	typedef iterable_iterator<adaptor_iterable<Adaptor>> iterator;

	iterator m_beginIterator;
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,end_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<end_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable);

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const end_action_tag&);

protected:
	typedef iterable_iterator<adaptor_iterable<Adaptor>> iterator;

	iterator m_endIterator;
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,forward_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<forward_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const forward_action_tag&);
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,backward_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<backward_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const backward_action_tag&);
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,displace_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<displace_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const displace_action_tag&);
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,remove_action_tag>
{
public:
	typedef mpl::type_pack<remove_action_tag> tags_t;
	typedef mpl::empty_type_pack const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	static constexpr inline auto perform_action(Adaptor& i_adaptor, const remove_action_tag&);
};

template<typename Adaptor,typename T>
class iterable_adaptor_action<Adaptor,add_action_tag<T>>
{
public:
	typedef mpl::type_pack<add_action_tag<T>> tags_t;
	typedef mpl::empty_type_pack const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	static constexpr inline auto perform_action(Adaptor& i_adaptor, add_action_tag<T>);
};

template<typename Adaptor,typename T>
class iterable_adaptor_action<Adaptor,agnostic_sink_action_tag<T>>
{
public:
	typedef typename mpl::which_type<mpl::is_const<T>,mpl::empty_type_pack,mpl::type_pack<agnostic_sink_action_tag<T>>>::type tags_t;
	typedef mpl::type_pack<agnostic_sink_action_tag<mpl::add_const<T>>> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	template<typename AAdaptor, typename Sink>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const sink_action_tag<Sink>& i_sink);
};

template<typename Adaptor>
class iterable_adaptor_action<Adaptor,size_action_tag>
{
public:
	typedef mpl::empty_type_pack tags_t;
	typedef mpl::type_pack<size_action_tag> const_tags_t;

	template<typename Iterable>
	iterable_adaptor_action(Iterable&& i_iterable) {};

	template<typename AAdaptor>
	static constexpr inline auto perform_action(AAdaptor&& i_adaptor, const size_action_tag&);
};

template<typename Iterable,typename ... IterableActions>
class iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>> : public iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions> ...
{
	template<typename,typename>
	friend class detail::iterable_adaptor_action;

	typedef iterable_value_type<Iterable> value_type;
	typedef iterable_reference<Iterable> reference;
	typedef iterable_const_reference<Iterable> const_reference;

public:
	typedef detail::iterable_by_type_traits<value_type,
		reference,
		const_reference,
		typename mpl::merge_type_packs<typename detail::iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions>::tags_t ...>::type,
		typename mpl::merge_type_packs<typename detail::iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions>::const_tags_t ...>::type> traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	using detail::iterable_adaptor_action<iterable_adaptor_actions<Iterable,mpl::type_pack<IterableActions...>>,IterableActions>::perform_action ...;

	iterable_adaptor_actions(Iterable& i_iterable);

private:
	typedef detail::iterable_iterator<Iterable> iterator;

	Iterable& m_iterable;
	mutable iterator m_currIterator;
};

template<typename Iterable>
class iterable_adaptor_actions<Iterable,mpl::type_pack<>>
{
public:
	typedef detail::iterable_by_type_traits<void,void,void,mpl::empty_type_pack,mpl::empty_type_pack> traits;
	typedef traits const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor_actions(Iterable& i_iterable)
	{
	}
};

}
}

#include "ddk_iterable_adaptor.inl"