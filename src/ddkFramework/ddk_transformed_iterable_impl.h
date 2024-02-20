#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Traits,typename Iterable>
inline auto transform_iterable(Iterable&& i_iterable);

template<typename Transform>
class iterable_transform
{
public:
    TEMPLATE(typename TTransform)
    REQUIRES(IS_CONSTRUCTIBLE(Transform,TTransform))
    iterable_transform(TTransform&& i_transform);

	const Transform& get_transform() const;

private:
	const Transform m_transform;
};

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

    TEMPLATE(typename IIterable, typename TTransform)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Transform,TTransform))
    transformed_iterable_impl(IIterable&& i_iterable, TTransform&& i_transform);

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

public:
	typedef typename PublicTraits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(Iterable& i_iterable, const Transform& i_tranform);

	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,typename traits::const_reference))
	constexpr auto perform_action(const sink_action_tag<Sink>& i_actionTag);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,typename traits::const_reference))
	constexpr auto perform_action(const sink_action_tag<Sink>& i_actionTag) const;
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,typename traits::const_reference))
	constexpr auto perform_action(sink_action_tag<Sink>&& i_actionTag);
	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,typename traits::const_reference))
	constexpr auto perform_action(sink_action_tag<Sink>&& i_actionTag) const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag);
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag) const;

private:
	mutable deduced_adaptor<Iterable> m_adaptor;
	const Transform m_transform;
};

template<typename PublicTraits,typename PrivateTraits,typename Iterable,typename Transform>
class iterable_adaptor<const detail::transformed_iterable_impl<PublicTraits,PrivateTraits,Iterable,Transform>>
{
	typedef PrivateTraits private_traits;

public:
	typedef typename Iterable::traits traits;
	typedef detail::const_iterable_traits<traits> const_traits;
	typedef typename traits::tags_t tags_t;
	typedef typename traits::const_tags_t const_tags_t;

	iterable_adaptor(const Iterable& i_iterable, const Transform& i_transform);

	TEMPLATE(typename Sink)
	REQUIRES(IS_CALLABLE_BY(Sink,typename traits::const_reference))
	constexpr auto perform_action(sink_action_tag<Sink>&& i_actionTag) const;
	TEMPLATE(typename ActionTag)
	REQUIRES(ACTION_TAGS_SUPPORTED(const_traits,ActionTag))
	constexpr auto perform_action(ActionTag&& i_actionTag) const;

private:
	deduced_adaptor<const Iterable> m_adaptor;
	const Transform m_transform;
};

}

#include "ddk_transformed_iterable_impl.inl"