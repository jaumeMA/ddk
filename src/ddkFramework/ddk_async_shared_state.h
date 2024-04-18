#pragma once

#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_arena.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_smart_pointer_concepts.h"
#include "ddk_async_executor_interface.h"
#include "ddk_weak_pointer_wrapper.h"
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

struct private_async_state_base
{
public:
	virtual ~private_async_state_base() = default;
};

using private_async_state_base_shared_ref = shared_reference_wrapper<private_async_state_base>;
using private_async_state_base_const_shared_ref = shared_reference_wrapper<const private_async_state_base>;
using private_async_state_base_shared_ptr = shared_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_shared_ptr = shared_pointer_wrapper<const private_async_state_base>;

using private_async_state_base_weak_ptr = weak_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_weak_ptr = weak_pointer_wrapper<const private_async_state_base>;

using private_async_state_base_lent_ref = lent_reference_wrapper<private_async_state_base>;
using private_async_state_base_const_lent_ref = lent_reference_wrapper<const private_async_state_base>;
using private_async_state_base_lent_ptr = lent_pointer_wrapper<private_async_state_base>;
using private_async_state_base_const_lent_ptr = lent_pointer_wrapper<const private_async_state_base>;

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
	typedef typename async_interface_base::cancel_result cancel_result;

	constexpr private_async_state();
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	constexpr inline private_async_state(Args&& ... i_args);
	~private_async_state() = default;
	constexpr cancel_result cancel();
	constexpr void attach(async_base_dist_ptr i_executor);
	constexpr bool detach();
	template<typename Predicate>
	constexpr bool detach_if(Predicate&& i_predicate);
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
	constexpr async_base_dist_ptr get_async_execution() const;

private:
	mutable mutex m_mutex;
	mutable mutex m_asyncMutex;
	mutable cond_var m_condVar;
	variant<detail::none_t,async_exception,T> m_arena;
	function<bool()> m_valuePredicate;
	mutable async_base_dist_ptr m_asyncExecutor;
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