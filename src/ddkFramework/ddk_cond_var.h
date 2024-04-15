#pragma once

#include <pthread.h>
#include "ddk_mutex.h"
#include "ddk_function.h"
#include <chrono>

namespace ddk
{

enum class CondVarAccess
{
	Local,
	Shared,
	Default = Local
};

class cond_var
{
public:
	cond_var(CondVarAccess i_access = CondVarAccess::Default);
	~cond_var();

	void wait(mutex& i_mutex);
	bool wait_for(mutex& i_mutex,const std::chrono::nanoseconds& i_time);
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable),IS_RETURN_TYPE_CONVERTIBLE_TO(Callable,bool))
	void wait_until(mutex& i_mutex, Callable&& i_predicate);
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable),IS_RETURN_TYPE_CONVERTIBLE_TO(Callable,bool))
	void wait_until(mutex& i_mutex, Callable&& i_predicate, const std::chrono::nanoseconds& i_time);
	void notify_all();
	void notify_one();
	CondVarAccess access() const;

private:
	mutable pthread_cond_t m_condVar;
	pthread_condattr_t m_attr;
};

}

#include "ddk_cond_var.inl"