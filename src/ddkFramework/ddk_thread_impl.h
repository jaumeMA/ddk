//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_execution_context.h"
#include "ddk_function.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_thread_utils.h"
#include "ddk_yielder.h"
#include "ddk_thread_local.h"
#include "ddk_this_thread.h"
#include "ddk_result.h"
#include <pthread.h>

namespace ddk
{
namespace detail
{

class thread_impl_interface
{
public:
	enum StartErrorCode
	{
		StartNoCallable,
		StartNotAvailable
	};
	typedef ddk::error<StartErrorCode> start_error;
	typedef result<void,start_error> start_result;

	enum StopErrorCode
	{
		NotRunning,
		NotStoppable
	};
	typedef ddk::error<StopErrorCode> stop_error;
	typedef ddk::result<void,stop_error> stop_result;

	virtual ~thread_impl_interface() = default;
	virtual start_result start(const ddk::function<void()>&, yielder* = nullptr) = 0;
	virtual stop_result stop() = 0;
	virtual bool joinable() const = 0;
	virtual thread_id_t get_id() const = 0;
	virtual void setExiting(bool i_exiting) = 0;
	virtual void execute() = 0;
	virtual bool set_affinity(const cpu_set_t&) = 0;

	static yielder* get_yielder();
	static yielder* set_yielder(yielder* i_yielder);
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
	one_shot_thread_impl();
	~one_shot_thread_impl();

private:
	start_result start(const ddk::function<void()>&, yielder* i_yielder = nullptr) override;
	stop_result stop() override;
	bool joinable() const override;
	thread_id_t get_id() const override;
	void setExiting(bool i_exiting) override;
	void execute() override;
	bool set_affinity(const cpu_set_t& i_set) override;

	pthread_t				m_thread;
	ddk::function<void()>	m_threadFunc;
	yielder*				m_yielder = nullptr;
	bool					m_started = false;
};

}
}
