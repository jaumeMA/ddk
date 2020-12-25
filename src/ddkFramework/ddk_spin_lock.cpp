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

}