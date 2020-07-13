#pragma once

#include <functional>
#include <thread>
#include "unique_reference_wrapper.h"
#include "lent_reference_wrapper.h"
#include <pthread.h>
#include "thread_utils.h"
#include "ddk_thread_yielder.h"
#include "ddk_fiber_yielder.h"
#include "ddk_thread_local.h"

namespace ddk
{

namespace detail
{

class thread_impl_interface
{
	static threadlocal<yielder_lent_ptr,thread_impl_interface>& get_yielder_local();

public:
	virtual ~thread_impl_interface() = default;
	virtual void start(const std::function<void()>&, yielder_lent_ptr = nullptr) = 0;
	virtual void stop() = 0;
	virtual bool joinable() const = 0;
	virtual thread_id_t get_id() const = 0;
	virtual void setExiting(bool i_exiting) = 0;
	virtual void execute() = 0;

	static yielder_lent_ptr get_yielder();
	static yielder_lent_ptr set_yielder(yielder_lent_ptr i_yielder);
	static void clear_yielder();
};

typedef unique_reference_wrapper<thread_impl_interface> thread_impl_unique_ref;
typedef unique_reference_wrapper<const thread_impl_interface> thread_impl_const_unique_ref;
typedef unique_pointer_wrapper<thread_impl_interface> thread_impl_unique_ptr;
typedef unique_pointer_wrapper<const thread_impl_interface> thread_impl_const_unique_ptr;

typedef lent_reference_wrapper<thread_impl_interface> thread_impl_lent_ref;
typedef lent_reference_wrapper<const thread_impl_interface> thread_impl_const_lent_ref;
typedef lent_pointer_wrapper<thread_impl_interface> thread_impl_lent_ptr;
typedef lent_pointer_wrapper<const thread_impl_interface> thread_impl_const_lent_ptr;

void* threadFunc(void *ptr);
void threadExiting(void* ptr);

//default version
class one_shot_thread_impl : public thread_impl_interface
{
public:
	one_shot_thread_impl() = default;

private:
	void start(const std::function<void()>&, yielder_lent_ptr i_yielder = nullptr) override;
	void stop() override;
	bool joinable() const override;
	thread_id_t get_id() const override;
	void setExiting(bool i_exiting) override;
	void execute() override;

	pthread_t				m_thread;
	std::function<void()>	m_threadFunc;
	yielder_lent_ptr		m_yielder;
	bool					m_started = false;
};

struct this_thread_t
{
public:
	typedef thread_id_t id;

	this_thread_t();
	id get_id() const;

private:
	pthread_t m_thread;
};

}
}
