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
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<typename Traits,typename Iterable>
inline auto transform_iterable(Iterable&& i_iterable);

template<typename,typename,typename>
struct iterable_transformed_action;

template<typename Reference,typename Transform, typename ActionTag>
struct iterable_transformed_action
{
public:
	iterable_transformed_action(const Transform& i_transform, ActionTag i_actionTag);

	auto operator*();
	template<typename T>
	constexpr inline Reference operator()(T&& i_args) const;

private:
	const Transform m_transform;
	ActionTag m_actionTag;
};
template<typename Reference,typename Transform, typename Sink>
struct iterable_transformed_action<Reference,Transform,sink_action_tag<Sink>>
{
public:
	template<typename SSink>
	iterable_transformed_action(const Transform& i_transform,SSink&& i_actionTag);
	~iterable_transformed_action();

	auto operator*();
	template<typename T>
	constexpr inline Reference operator()(T&& i_args) const;

private:
	const Transform m_transform;
	sink_action_tag<Sink> m_actionTag;
	mutable typed_arena<Reference> m_cache;
};

template<typename Transform>
class iterable_transform
{
public:
	TEMPLATE(typename TTransform)
    REQUIRES(IS_CONSTRUCTIBLE(Transform,TTransform))
    iterable_transform(TTransform&& i_transform);

	template<typename Reference,typename ActionTag>
	auto map_action(ActionTag&& i_action) const;

private:
	const Transform m_transform;
};
template<typename Transform>
iterable_transform(const Transform&) -> iterable_transform<Transform>;
template<typename Transform>
iterable_transform(Transform&&) -> iterable_transform<Transform>;

template<typename FromTraits,typename ToTraits>
class traits_conversion_callable
{
public:
    typedef typename FromTraits::value_type from_value_type;
    typedef typename ToTraits::value_type to_value_type;
    typedef typename FromTraits::reference from_reference;
    typedef typename ToTraits::reference to_reference;
    typedef typename FromTraits::const_reference from_const_reference;
    typedef typename ToTraits::const_reference to_const_reference;

    inline to_reference operator()(from_reference i_value) const;
	TEMPLATE(typename T)
	REQUIRES(IS_CONVERTIBLE(T,to_const_reference))
	inline to_const_reference operator()(T&& i_value, ...) const; //added variadic for avoiding clash when from_reference and from_const_reference are the same
};

template<typename PublicTraits, typename PrivateTraits, typename Iterable, typename Transform>
class transformed_iterable_impl : public iterable_impl_interface<PublicTraits>, public iterable_visitor<transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>
{
	typedef iterable_visitor<transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>> base_t;

public:
    typedef PublicTraits traits;
	typedef detail::const_iterable_traits<traits> const_traits;

    TEMPLATE(typename IIterable)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable))
    transformed_iterable_impl(IIterable&& i_iterable, const ddk::detail::iterable_transform<Transform>& i_transform);

    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(traits,Action))
    void iterate_impl(Action&& i_initialAction);
    TEMPLATE(typename Action)
    REQUIRES(ACTION_SUPPORTED(const_traits,Action))
    void iterate_impl(Action&& i_initialAction) const;
};

}

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
class iterable_adaptor<detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>
{
	typedef PrivateTraits private_traits;
	typedef detail::const_iterable_traits<private_traits> private_const_traits;
	template<typename Adaptor>
	using private_adaptor_traits = typename mpl::which_type<mpl::is_const<Adaptor>,private_const_traits,private_traits>::type;

public:
	typedef PublicTraits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterable& i_iterable, const detail::iterable_transform<Transform>& i_tranform);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static constexpr auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	mutable deduced_adaptor<Iterable> m_adaptor;
	const ddk::detail::iterable_transform<Transform> m_transform;
};

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
class iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>
{
	typedef PrivateTraits private_traits;
	typedef detail::const_iterable_traits<private_traits> private_const_traits;
	template<typename Adaptor>
	using private_adaptor_traits = typename mpl::which_type<mpl::is_const<Adaptor>,private_const_traits,private_traits>::type;

public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterable& i_iterable, const detail::iterable_transform<Transform>& i_transform);

	TEMPLATE(typename Adaptor, typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(Adaptor,ActionTag))
	static constexpr auto perform_action(Adaptor&& i_adaptor, ActionTag&& i_actionTag);

private:
	deduced_adaptor<const Iterable> m_adaptor;
	const ddk::detail::iterable_transform<Transform> m_transform;
};

}

#include "ddk_transformed_iterable_impl.inl"