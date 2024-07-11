#include "ddk_mutex.h"

namespace ddk
{

mutex::mutex(MutexType i_mutexType, MutexAccess i_mutexAccess)
{
	pthread_mutexattr_init(&m_attr);
	switch(i_mutexType)
	{
		case MutexType::Normal:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_NORMAL);
			break;
		}
		case MutexType::Recursive:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_RECURSIVE);
			break;
		}
		case MutexType::ErrorCheck:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_ERRORCHECK);
			break;
		}
	}

	if (i_mutexAccess == MutexAccess::Shared)
	{
		pthread_mutexattr_setpshared(&m_attr,PTHREAD_PROCESS_SHARED);
	}

	pthread_mutex_init(&m_mutex,&m_attr);
}
mutex::mutex(const mutex& other)
: mutex()
{
	pthread_mutexattr_init(&m_attr);
	int _type = 0;
	pthread_mutexattr_gettype(&other.m_attr,&_type);

	switch (static_cast<MutexType>(_type))
	{
		case MutexType::Normal:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_NORMAL);
			break;
		}
		case MutexType::Recursive:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_RECURSIVE);
			break;
		}
		case MutexType::ErrorCheck:
		{
			pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_ERRORCHECK);
			break;
		}
	}

	int _access = 0;
	pthread_mutexattr_getpshared(&other.m_attr,&_access);

	if (static_cast<MutexAccess>(_access) == MutexAccess::Shared)
	{
		pthread_mutexattr_setpshared(&m_attr,PTHREAD_PROCESS_SHARED);
	}

	pthread_mutex_init(&m_mutex,&m_attr);
}
mutex::~mutex()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_mutexattr_destroy(&m_attr);
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
MutexType mutex::type() const
{
	int _type = 0;

	pthread_mutexattr_gettype(&m_attr,&_type);

	return static_cast<MutexType>(_type);
}
MutexAccess mutex::access() const
{
	int _access = 0;

	pthread_mutexattr_getpshared(&m_attr,&_access);

	return static_cast<MutexAccess>(_access);
}
pthread_mutex_t* mutex::get_impl()
{
	return &m_mutex;
}

}