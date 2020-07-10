#pragma once

#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#else
#include <chrono>

#endif

#include <functional>
#include "thread_utils.h"
#include "ewas_thread_impl.h"

namespace ewas
{

class thread
{
	friend void* threadFunc(void*);
	friend void threadExiting(void*);

public:
	typedef thread_id_t id;

	thread();
	thread(detail::thread_impl_unique_ref i_threadImpl);
	thread(const thread&) = delete;
	thread(thread&&);
	~thread();
	thread& operator=(const thread&) = delete;
	thread& operator=(thread&&);

	id get_id() const;	
	void start(const std::function<void()>& i_threadFunc, detail::yielder_lent_ptr i_yielder = nullptr);
	void stop();
	bool joinable() const;

private:
	detail::thread_impl_unique_ref m_threadImpl;
};

extern const detail::this_thread_t this_thread;

}
