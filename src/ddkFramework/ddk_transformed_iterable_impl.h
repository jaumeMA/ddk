#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

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
    inline to_const_reference operator()(from_const_reference i_value) const;
};

template<typename PublicTraits, typename PrivateTraits, typename Iterable, typename Transform>
class transformed_iterable_impl : public iterable_impl_interface<PublicTraits>
{
    typedef PrivateTraits private_traits;
    typedef typename private_traits::value_type private_value_type;
    typedef typename private_traits::reference private_reference;
    typedef typename private_traits::const_reference private_const_reference;

    friend inline auto deduce_adaptor(const transformed_iterable_impl& i_iterableImpl)
    {
        return deduce_adaptor(i_iterableImpl.m_iterable);
    }


public:
    typedef PublicTraits traits;
    typedef typename traits::value_type value_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;

    TEMPLATE(typename IIterable, typename TTransform)
    REQUIRES(IS_CONSTRUCTIBLE(Iterable,IIterable),IS_CONSTRUCTIBLE(Transform,TTransform))
    transformed_iterable_impl(IIterable&& i_iterable, TTransform&& i_transform);

    TEMPLATE(typename Function, typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,reference),TRANSFORMED_ACTION_TAGS_SUPPORTED(PublicTraits,Function))
    void iterate_impl(Function&& i_try, const Action& i_initialAction);
    TEMPLATE(typename Function, typename Action)
    REQUIRES(IS_CALLABLE_BY(Function,const_reference),TRANSFORMED_ACTION_TAGS_SUPPORTED(PublicTraits,Function))
    void iterate_impl(Function&& i_try, const Action& i_initialAction) const;
    template<typename ... Args>
    auto operator()(Args&& ... i_args) const;

private:
    Iterable m_iterable;
    const Transform m_transform;
};

}
}

#include "ddk_transformed_iterable_impl.inl"
