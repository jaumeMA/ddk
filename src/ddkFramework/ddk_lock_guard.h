#pragma once

#include "ddk_mutex.h"

namespace ddk
{

class lock_guard
{
public:
	lock_guard(mutex& i_mutex);
	~lock_guard();

private:
	mutex& m_mutex;
};

}