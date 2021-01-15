#pragma once

#include "ddk_atomics.h"

namespace ddk
{

class spin_lock
{
public:
	spin_lock();

	void lock();
	void unlock();
	bool try_lock();
	bool is_locked() const;

private:
	mutable atomic_bool m_barrier;
};

}
