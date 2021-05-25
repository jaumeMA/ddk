#include "ddk_spin_lock.h"
#include <thread>

namespace ddk
{

spin_lock::spin_lock()
: m_barrier(false)
{
}
void spin_lock::lock()
{
	while(atomic_compare_exchange(m_barrier,false,true) == false) { std::this_thread::yield(); }
}
void spin_lock::unlock()
{
	while(atomic_compare_exchange(m_barrier,true,false) == false) { std::this_thread::yield(); }
}
bool spin_lock::try_lock()
{
	return atomic_compare_exchange(m_barrier,false,true);
}
bool spin_lock::is_locked() const
{
	return m_barrier.get();
}

exclusive_spin_lock::exclusive_spin_lock()
: m_barrier(0)
{
}
void exclusive_spin_lock::lock()
{
	size_t currBarrier = 0;
	do
	{
		currBarrier = m_barrier.get() & ~s_writer_mask;

		if(atomic_compare_exchange(m_barrier,currBarrier,currBarrier + 1))
		{
			break;
		}

		std::this_thread::yield();
	}
	while(true);
}
void exclusive_spin_lock::unlock()
{
	size_t currBarrier = 0;
	do
	{
		currBarrier = m_barrier.get();

		if(atomic_compare_exchange(m_barrier,currBarrier,currBarrier - 1))
		{
			break;
		}
	
		std::this_thread::yield();
	}
	while(true);
}
void exclusive_spin_lock::lock_exclusive(bool i_withPriority)
{
	do
	{
		size_t currBarrier = m_barrier.get();

		if((currBarrier & ~s_waiting_writer_bit) == 0 && atomic_compare_exchange(m_barrier,currBarrier,s_writer_bit))
		{
			break;
		}

		if(i_withPriority && (currBarrier & s_waiting_writer_bit) == 0)
		{
			atomic_compare_exchange(m_barrier,currBarrier,currBarrier | s_waiting_writer_bit);
		}

		std::this_thread::yield();

	} while(true);
}
void exclusive_spin_lock::unlock_exclusive()
{
	m_barrier.set(m_barrier.get() & s_waiting_writer_bit);
}
bool exclusive_spin_lock::is_locked() const
{
	return m_barrier.get() != 0;
}

}