#include "ddk_mutex.h"

namespace ddk
{

mutex::mutex(MutexType i_mutexType)
{
	pthread_mutexattr_t mutexAttr;

	pthread_mutexattr_init(&mutexAttr);
	switch(i_mutexType)
	{
		case MutexType::Normal:
		{
			pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_NORMAL);
			break;
		}
		case MutexType::Recursive:
		{
			pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_RECURSIVE);
			break;
		}
		case MutexType::ErrorCheck:
		{
			pthread_mutexattr_settype(&mutexAttr,PTHREAD_MUTEX_ERRORCHECK);
			break;
		}
	}

	pthread_mutex_init(&m_mutex,&mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
}
mutex::mutex(const mutex&)
: mutex()
{
}
mutex::mutex(mutex&&)
: mutex()
{
}
mutex::~mutex()
{
	pthread_mutex_destroy(&m_mutex);
}
void mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}
bool mutex::try_lock()
{
	return pthread_mutex_trylock(&m_mutex) == 0;
}
void mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}
pthread_mutex_t* mutex::get_impl()
{
	return &m_mutex;
}

}