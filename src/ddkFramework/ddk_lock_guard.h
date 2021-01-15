#pragma once

#include "ddk_mutex.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename ... TT>
class lockable_guard;

template<typename T>
class lockable_guard<T>
{
public:
	lockable_guard(T& i_lockableObject);
	~lockable_guard();

private:
	T& m_lockableObject;
};

template<typename T, typename ... TT>
class lockable_guard<T,TT...>
{
    static const size_t s_numSecondaryLocks = mpl::get_num_types<TT...>();

public:
	lockable_guard(T& i_lockableObject, TT& ... i_lockableObjects);
	~lockable_guard();

private:
    T& m_primaryLock;
	void* m_secondaryLocks[s_numSecondaryLocks] = { nullptr };
};

typedef lockable_guard<mutex> lock_guard;
}

#include "ddk_lock_guard.inl"
