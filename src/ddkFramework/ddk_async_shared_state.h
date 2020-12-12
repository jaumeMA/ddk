#pragma once

#include <pthread.h>
#include "ddk_arena.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_async_executor_interface.h"
#include "ddk_variant.h"
#include "ddk_lend_from_this.h"

namespace ddk
{
namespace detail
{

struct async_state_base : public lend_from_this<async_state_base>
{
	virtual ~async_state_base() = default;
	virtual void notify() = 0;
};

template<typename T>
struct private_async_state : async_state_base
{
	template<typename>
	friend struct private_async_state;

public:
	typedef typename mpl::static_if<std::is_reference<T>::value,typename embedded_type<T>::ref_type,typename embedded_type<T>::cref_type>::type sink_type;
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rref_type;
	typedef typename async_cancellable_interface::cancel_result cancel_result;

	private_async_state();
	~private_async_state();
	cancel_result cancel();
	void attach(async_cancellable_shared_ptr i_executor);
	void detach();
	void set_value(sink_type i_value);
	void set_exception(const async_exception& i_exception);
	void signal() const;
	const_reference get_value() const;
	reference get_value();
	embedded_type<T> extract_value();
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;
	virtual void notify() override;

private:
	mutable pthread_mutex_t m_mutex;
	mutable pthread_cond_t m_condVar;
	variant<detail::none_t,async_exception,T> m_arena;
	async_cancellable_shared_ptr m_asyncExecutor;
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
using private_async_state_lent_ref = lent_reference_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ref = lent_reference_wrapper<const private_async_state<T>>;
template<typename T>
using private_async_state_lent_ptr = lent_pointer_wrapper<private_async_state<T>>;
template<typename T>
using private_async_state_const_lent_ptr = lent_pointer_wrapper<const private_async_state<T>>;

}
}

#include "ddk_async_shared_state.inl"
