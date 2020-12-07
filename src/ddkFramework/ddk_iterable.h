#pragma once

#if defined(WIN32)

#define _ENABLE_EXTENDED_ALIGNED_STORAGE

#endif

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_iterable_valued_traits.h"
#include "ddk_iterable_state.h"
#include "ddk_await_executor.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable : protected iterable_interface
{
    template<typename TTraits>
    friend class iterable;
    friend inline iterable_impl_lent_ref<typename Traits::iterable_base_traits> lend(const iterable<Traits>& i_iterable)
    {
        return lend(i_iterable.m_iterableImpl);
    }
    friend inline iterable_impl_shared_ref<typename Traits::iterable_base_traits> share(const iterable<Traits>& i_iterable)
    {
        return i_iterable.m_iterableImpl;
    }

public:
    typedef typename Traits::iterable_value iterable_value;
    typedef typename Traits::iterable_const_value iterable_const_value;
    typedef typename Traits::iterable_base_traits iterable_base_traits;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;
    typedef Traits traits;

    iterable(iterable_impl_shared_ref<iterable_base_traits> i_iterableImpl);
    iterable(const iterable&);
    template<typename TTraits>
    iterable(const iterable<TTraits>& other);
	~iterable();

    iter::action_result iterate(const function<void(reference)>& i_try, const iter::shift_action& i_initialAction = iter::go_to_place);
    iter::action_result iterate(const function<void(const_reference)>& i_try, const iter::shift_action& i_initialAction = iter::go_to_place) const;
	iter::action_result co_iterate(const function<void(iterable_value)>& i_try,const iter::shift_action& i_initialAction = iter::go_to_place);
	iter::action_result co_iterate(const function<void(iterable_const_value)>& i_try,const iter::shift_action& i_initialAction = iter::go_to_place) const;
	bool inline forward_action(action i_action) const;
    bool inline operator==(const std::nullptr_t&) const;
    bool inline operator!=(const std::nullptr_t&) const;
    size_t inline size() const;
    bool inline empty() const;

private:
    iter::iterable_state& get_state() override;
    const iter::iterable_state& get_state() const override;

    action private_iterate(reference i_value);
    action private_iterate(const_reference i_value) const;
    reference resolve_action(const action&);
    const_reference resolve_action(const action&) const;

	mutable lendable<iter::action_state> m_actionState;
    iterable_impl_shared_ref<iterable_base_traits> m_iterableImpl;
    mutable iter::iterable_state m_iterableState;
	mutable detail::await_executor<void> m_executor;
	mutable typed_arena<reference> m_iterableValueContainer;
    mutable action m_currAction = iterable_base_traits::default_action();
};

}

template<typename T>
using const_forward_iterable = detail::iterable<detail::const_forward_iterable_valued_traits<T>>;

template<typename T>
using forward_iterable = detail::iterable<detail::forward_iterable_valued_traits<T>>;

template<typename T>
using const_bidirectional_iterable = detail::iterable<detail::const_bidirectional_iterable_valued_traits<T>>;

template<typename T>
using bidirectional_iterable = detail::iterable<detail::bidirectional_iterable_valued_traits<T>>;

template<typename T>
using const_random_access_iterable = detail::iterable<detail::const_random_access_iterable_valued_traits<T>>;

template<typename T>
using random_access_iterable = detail::iterable<detail::random_access_iterable_valued_traits<T>>;

}

#include "ddk_iterable.inl"

#include "ddk_iterable_utils.h"
#include "ddk_view_utils.h"