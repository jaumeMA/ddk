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

template<typename T>
struct private_async_state
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
	typedef typename async_cancellable_interface::cancel_result cancel_result;

	private_async_state();
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	inline private_async_state(Args&& ... i_args);
	virtual ~private_async_state() = default;
	cancel_result cancel();
	void attach(async_cancellable_dist_ptr i_executor);
	bool detach();
	template<typename Predicate>
	bool detach_if(Predicate&& i_predicate);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	inline void emplace(Args&& ... i_args);
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void signal() const;
	const_reference get_value() const;
	reference get_value();
	embedded_type<T> extract_value() &&;
	void clear();
	void wait() const;
	void wait_for(const std::chrono::milliseconds& i_period) const;
	bool ready() const;

	async_cancellable_dist_ptr get_async_execution() const;

private:
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
	variant<detail::none_t,async_exception,T> m_arena;
	function<bool()> m_valuePredicate;
	mutable async_cancellable_dist_ptr m_asyncExecutor;
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