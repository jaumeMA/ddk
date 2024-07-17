//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pthread.h>

namespace ddk
{

enum class MutexType
{
	Normal,
	Recursive,
	ErrorCheck,
	Default = Normal
};

enum class MutexAccess
{
	Local,
	Shared,
	Default = Local
};

class mutex
{
	friend class cond_var;

public:
	mutex(MutexType i_mutexType = MutexType::Default, MutexAccess i_mutexAccess = MutexAccess::Default);
	mutex(const mutex& other);
	~mutex();

	mutex& operator=(const mutex&) = delete;
	void lock();
	bool try_lock();
	void unlock();
	MutexType type() const;
	MutexAccess access() const;

private:
	pthread_mutex_t* get_impl();

	pthread_mutex_t m_mutex;
	pthread_mutexattr_t m_attr;

};

}