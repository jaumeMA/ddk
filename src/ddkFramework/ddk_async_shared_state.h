//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_async_shared_state_base.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_arena.h"
#include "ddk_variant.h"
#include "ddk_lend_from_this.h"

namespace ddk
{

template<typename>
class promise;
template<typename,typename>
class executor_promise;

namespace detail
{

template<typename T>
struct private_async_state : public private_async_state_base
{
	template<typename>
	friend struct private_async_state;
	template<typename>
	friend class ddk::promise;
	template<typename,typename>
	friend class ddk::executor_promise;

public:
	typedef typename mpl::static_if<std::is_reference<T>::value,typename embedded_type<T>::ref_type,typename mpl::static_if<std::is_copy_constructible<T>::value,embedded_type<T>,embedded_type<T&&>>::type::cref_type>::type sink_type;
	typedef typename embedded_type<T>::internal_type value_type;
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rreference;

	constexpr private_async_state();
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	constexpr inline private_async_state(Args&& ... i_args);
	~private_async_state() = default;
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	constexpr inline void emplace(Args&& ... i_args);
	constexpr void set_value(sink_type i_value);
	constexpr void set_exception(const async_exception& i_exception);
	constexpr void signal() const;
	constexpr const_reference get_value() const;
	constexpr reference get_value();
	constexpr embedded_type<T> extract_value() &&;
	constexpr void clear();
	constexpr void wait() const;
	constexpr void wait_for(const std::chrono::milliseconds& i_period) const;
	constexpr bool ready() const;

private:
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
	variant<detail::none_t,async_exception,T> m_arena;
	function<bool()> m_valuePredicate;
};

template<typename T>
using private_async_state_shared_ref = shared_reference_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_shared_ref = shared_reference_wrapper<const private_async_state<T>>;
template<typename T>
using private_async_state_shared_ptr = shared_pointer_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_shared_ptr = shared_pointer_wrapper<const private_async_state<T>>;

template<typename T>
using private_async_state_weak_ptr = weak_pointer_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_weak_ptr = weak_pointer_wrapper<const private_async_state<T>>;

template<typename T>
using private_async_state_lent_ref = lent_reference_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ref = lent_reference_wrapper<const private_async_state<T>>;
template<typename T>
using private_async_state_lent_ptr = lent_pointer_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ptr = lent_pointer_wrapper<const private_async_state<T>>;

}
}

#include "ddk_embedded_async_shared_state.h"
#include "ddk_context_async_shared_state.h"
#include "ddk_async_shared_state.inl"