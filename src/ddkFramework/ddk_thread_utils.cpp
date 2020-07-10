#include "thread_utils.h"

#ifdef WIN32
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif

namespace ewas
{

thread_id_t get_current_thread_id()
{
	return CURR_THREAD_ID;
}

thread_id_t get_thread_id(pthread_t i_thread)
{
#if defined(WIN32)

  return pthread_getunique_np(i_thread);

#elif defined(__LINUX__)

  return i_thread;

#elif defined(__APPLE__)
    
    return reinterpret_cast<size_t>(i_thread);
    
#endif
}

pthread_t get_current_thread()
{
	return pthread_self();
}

void sleep(unsigned long milliseconds)
{
	#ifdef WIN32
	Sleep(milliseconds);
	#else
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	#endif
}

bool thread_id_equal(const thread_id_t& i_lhs, const thread_id_t& i_rhs)
{
#if defined(WIN32)

  return i_lhs == i_rhs;

#elif defined(__linux__)

  return pthread_equal(i_lhs,i_rhs) != 0;

#elif defined(__APPLE__)
    
    return i_lhs == i_rhs;
    
#endif
}

}
