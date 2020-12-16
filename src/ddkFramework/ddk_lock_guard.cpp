#include "ddk_lock_guard.h"

namespace ddk
{

lock_guard::lock_guard(mutex& i_mutex)
: m_mutex(i_mutex)
{
	m_mutex.lock();
}
lock_guard::~lock_guard()
{
	m_mutex.unlock();
}

}