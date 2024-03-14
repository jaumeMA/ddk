#pragma once

#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include "ddk_mutex.h"

namespace ddk
{

template<typename T, typename Tag = T>
struct stacklocal
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	stacklocal(Args&& ... i_args);
	~stacklocal();

	T* operator->();

private:
	typedef std::map<size_t,T> stack_container;
	static const size_t nobject = -1;

	static stack_container s_stackedObjects;
	typename stack_container::iterator m_itStack;
	bool m_owner = false;
	mutex m_mutex;
};

}

#include "ddk_stack_local.inl"