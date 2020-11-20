#pragma once

#include <pthread.h>
#include "ddk_arena.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct private_async_state
{
	typedef typename mpl::static_if<std::is_reference<T>::value,T,const T&>::type sink_type;

public:
	typedef typename embedded_type<T>::ref_type reference;
	typedef typename embedded_type<T>::cref_type const_reference;
	typedef typename embedded_type<T>::rref_type rref_type;

	private_async_state();
	~private_async_state();
	void set_value(sink_type i_value);
	void signal();
	const_reference get_value() const;
	reference get_value();
	embedded_type<T> extract_value();
	void wait() const;
	void wait_for(unsigned int i_period) const;
	bool ready() const;

private:
	mutable pthread_mutex_t m_mutex;
	mutable pthread_cond_t m_condVar;
	typed_arena<T> m_arena;
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
