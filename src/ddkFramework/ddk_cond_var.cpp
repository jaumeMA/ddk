#include "ddk_cond_var.h"

namespace ddk
{

cond_var::cond_var()
{
	pthread_cond_init(&m_condVar,nullptr);
}
cond_var::~cond_var()
{
	pthread_cond_destroy(&m_condVar);
}
void cond_var::wait(mutex& i_mutex)
{
	pthread_cond_wait(&m_condVar,i_mutex.get_impl());
}
bool cond_var::wait_for(mutex& i_mutex, const std::chrono::nanoseconds& i_time)
{
	const std::chrono::seconds secs = std::chrono::duration_cast<std::chrono::seconds>(i_time);
	const struct timespec time_to_wait = { time(nullptr) + static_cast<time_t>(secs.count()), static_cast<long>((i_time - std::chrono::duration_cast<std::chrono::nanoseconds>(secs)).count()) };

	return pthread_cond_timedwait(&m_condVar,i_mutex.get_impl(),&time_to_wait) == 0;
}
void cond_var::wait_until(mutex& i_mutex,const function<bool()>& i_predicate)
{
	while(eval(i_predicate))
	{
		pthread_cond_wait(&m_condVar,i_mutex.get_impl());
	}
}
void cond_var::wait_until(mutex& i_mutex,const function<bool()>& i_predicate,const std::chrono::nanoseconds& i_time)
{
	while(eval(i_predicate))
	{
		const std::chrono::seconds secs(std::chrono::duration_cast<std::chrono::seconds>(i_time));
		const struct timespec time_to_wait = { time(nullptr) + static_cast<time_t>(secs.count()), static_cast<long>((i_time - std::chrono::duration_cast<std::chrono::nanoseconds>(secs)).count()) };

		pthread_cond_timedwait(&m_condVar,i_mutex.get_impl(),&time_to_wait);
	}
}
void cond_var::notify_all()
{
	pthread_cond_broadcast(&m_condVar);
}
void cond_var::notify_one()
{
	pthread_cond_signal(&m_condVar);
}

}