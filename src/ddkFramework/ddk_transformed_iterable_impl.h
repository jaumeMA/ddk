#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename PublicTraits, typename PrivateTraits>
class transformed_iterable_impl : public iterable_impl_interface<typename PublicTraits::iterable_base_traits>
{
    static_assert(std::is_same<typename PublicTraits::action,typename PrivateTraits::action>::value, "Actions from both public and private traits shall be the same");

    typedef typename PrivateTraits::iterable_base_traits private_iterable_base_traits;
    typedef typename PrivateTraits::reference private_reference;
    typedef typename PrivateTraits::const_reference private_const_reference;

public:
    typedef typename PublicTraits::value_type value_type;
    typedef typename PublicTraits::reference reference;
    typedef typename PublicTraits::const_reference const_reference;
    typedef typename PublicTraits::action action;

    transformed_iterable_impl(iterable_impl_shared_ref<private_iterable_base_traits> i_iterableRef, const function<reference(private_reference)>& i_transform);

private:
    void iterate_impl(const function<action(reference)>& i_try) override;
    void iterate_impl(const function<action(const_reference)>& i_try) const override;

    iterable_impl_shared_ref<private_iterable_base_traits> m_iterableRef;
    const function<reference(private_reference)> m_transform;
};

}
}

#include "ddk_transformed_iterable_impl.inl"